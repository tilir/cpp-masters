#!/usr/bin/env ruby
# frozen_string_literal: true

require 'open3'

if ARGV.size < 3
  warn "Usage:"
  warn "  #{$PROGRAM_NAME} MIN_READERS MAX_READERS label1 --cmd=... [args...] label2 --cmd=... [args...] ..."
  exit 1
end

min_readers = Integer(ARGV.shift)
max_readers = Integer(ARGV.shift)

Series = Struct.new(:label, :cmd, :args)

series = []

while ARGV.any?
  label = ARGV.shift
  args  = []
  args << ARGV.shift while ARGV.any? && ARGV[0].start_with?('-')

  cmd = nil
  clean_args = []

  i = 0
  while i < args.size
    tok = args[i]
    if tok == '--cmd'
      i += 1
      cmd = args[i] or abort "--cmd requires a value"
      i += 1
    elsif tok.start_with?('--cmd=')
      cmd = tok.sub('--cmd=', '')
      i += 1
    else
      clean_args << tok
      i += 1
    end
  end

  unless cmd
    warn "Series '#{label}' must specify --cmd=... or --cmd ... "
    exit 1
  end

  series << Series.new(label, cmd, clean_args)
end

# gnuplot header
plot_specs = series.map do |s|
  %Q{"-" using 1:2 with linespoints title "#{s.label}"}
end.join(", ")

puts <<~GNUPLOT
  set term pngcairo size 1500,600
  set grid
  set xlabel "Number of readers"
  set ylabel "RPS per reader"
  set title "Per-reader throughput vs readers"
  plot #{plot_specs}
GNUPLOT

# data blocks
series.each do |s|
  (min_readers..max_readers).each do |r|
    cmdline = [s.cmd, "--readers=#{r}", *s.args]
    stdout_str, stderr_str, status = Open3.capture3(*cmdline)

    rps = 0.0
    if status.success?
      line = stdout_str.lines.find { |l| l.start_with?('rps_per_reader=') }
      rps = line ? line.split('=', 2).last.to_f : 0.0
    else
      warn "Command failed for #{s.label} readers=#{r}: #{stderr_str}"
    end

    puts "#{r} #{rps}"
  end
  puts "e"
end
