#!/usr/bin/env ruby
#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Standalone include-what-you-use driver: works on a compilation database
# without CMake reconfiguration.
#
#   ruby tools/iwyu.rb [--check] [build-dir]
#
# build-dir defaults to build/clang-release and is configured on demand.
# Without --check the suggestions are applied with fix_include and the tree
# is left modified for review. modules/ and standalone projects are not part
# of the build, hence not checked.
#
#------------------------------------------------------------------------------
require 'open3'
require 'json'
require 'shellwords'

check = ARGV.delete('--check') ? true : false
abort 'usage: ruby tools/iwyu.rb [--check] [build-dir]' if ARGV.length > 1
build_dir = ARGV[0] || 'build/clang-release'

root, status = Open3.capture2('git', 'rev-parse', '--show-toplevel')
abort 'cannot locate repository' unless status.success?
Dir.chdir(root.strip)

local_bin = File.join(Dir.pwd, 'build/tools/iwyu/bin')
def tool_default(local_bin, local_name, system_name)
  local = File.join(local_bin, local_name)
  File.executable?(local) ? local : system_name
end

iwyu = ENV.fetch('IWYU_BINARY', tool_default(local_bin, 'include-what-you-use', 'include-what-you-use'))
# iwyu_tool interprets IWYU_BINARY as a pathname, not a command on PATH.
# Resolve bare command names before passing the override to that driver.
unless iwyu.include?(File::SEPARATOR)
  iwyu = ENV.fetch('PATH', '').split(File::PATH_SEPARATOR).map { |dir|
    File.expand_path(iwyu, dir)
  }.find { |path| File.file?(path) && File.executable?(path) }
end
abort 'include-what-you-use not found (set IWYU_BINARY)' unless
  iwyu && File.file?(iwyu) && File.executable?(iwyu)
iwyu = File.realpath(iwyu)
version, status = Open3.capture2(iwyu, '--version')
abort 'IWYU 0.25 built against Clang 21 is required (set IWYU_BINARY)' unless
  status.success? && version.match?(/include-what-you-use 0\.25 .*clang version 21\./i)
ENV['IWYU_BINARY'] = iwyu

database = File.join(build_dir, 'compile_commands.json')
unless File.exist?(database)
  abort "missing #{database}; configure this build with CMAKE_EXPORT_COMPILE_COMMANDS=ON" if ARGV[0]
  system('cmake', '--preset', 'clang-release', exception: true)
end
entries = JSON.parse(File.read(database))
abort 'empty compilation database' if entries.empty?
unless entries.all? { |entry| File.basename((entry['arguments'] || Shellwords.split(entry.fetch('command'))).first) == 'clang++-21' }
  abort 'use a clang++-21 compilation database for IWYU'
end

iwyu_args = ['-Xiwyu', "--mapping_file=#{Dir.pwd}/tools/iwyu.imp",
             '-Xiwyu', '--no_fwd_decls',
             '-Xiwyu', "--check_also=#{Dir.pwd}/*.hpp"]
driver = ENV.fetch('IWYU_TOOL', tool_default(local_bin, 'iwyu_tool.py', 'iwyu_tool'))
command = [driver, '-p', build_dir, '-j', ENV.fetch('IWYU_JOBS', '2'), '--', *iwyu_args]

if check
  # --error reports violations; --error_always would also fail a clean run.
  system(*command, '-Xiwyu', '--error', exception: true)
  puts 'iwyu check: OK'
else
  out, status = Open3.capture2e(*command)
  abort out unless status.success?
  puts Open3.capture2(ENV.fetch('FIX_INCLUDE', tool_default(local_bin, 'fix_includes.py', 'fix_include')),
                     '--nosafe_headers', '--reorder', stdin_data: out).tap { |_, result|
    abort 'fix_include failed' unless result.success?
  }.first
  puts 'iwyu suggestions applied, review the diff and run tools/format.rb'
end
