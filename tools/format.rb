#!/usr/bin/env ruby
#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Format the tree with clang-format-21, or check it with --check.
# modules/, concurrency/follybenches/ and concurrency/tsv2/ are excluded
# (they carry their own .clang-format with DisableFormat: true).
#
#------------------------------------------------------------------------------
require 'open3'

EXCLUDED = %r{\A(modules/|concurrency/follybenches/|concurrency/tsv2/)}.freeze

root, = Open3.capture2('git', 'rev-parse', '--show-toplevel')
Dir.chdir(root.strip)

clang_format = ENV.fetch('CLANG_FORMAT', 'clang-format-21')
version, status = Open3.capture2(clang_format, '--version')
abort 'clang-format 21 is required' unless status.success? && version.match?(/version 21\./)

files, = Open3.capture2('git', 'ls-files', '*.cc', '*.hpp', '*.h')
files = files.split("\n").reject { |f| f.match?(EXCLUDED) }

if ARGV[0] == '--check'
  system(clang_format, '--dry-run', '--Werror', *files,
         exception: true, err: $stderr)
  puts 'format check: OK'
else
  system(clang_format, '-i', *files, exception: true)
  puts 'formatted in place'
end
