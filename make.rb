#!/usr/bin/env ruby

require 'beaver'
require 'fileutils'
require 'colorize'

$beaver.set :e

CFLAGS = "-g -Ideps/olive.c"

OUT = "out"
OBJ_OUT = File.join(OUT, "obj")

[OUT, OBJ_OUT].each do |dir|
  if !Dir.exist? dir
    FileUtils.mkdir_p dir
  end
end

cmd :build do
  call :build_obj
end

cmd :build_obj, each(["src/*.c"]) do
  sh %(clang -c #{CFLAGS} #{$file} -o #{File.join(OBJ_OUT, $file.name + ".o")})
end

cmd :staticlib do
  puts "unimplemented"
end

cmd :dynlib do
  puts "unimplemented"
end

cmd :test do
  call :build
  sh %(clang #{CFLAGS} #{`pkg-config sdl2 --cflags`.gsub("\n", "")} test.c out/obj/*.o #{`pkg-config sdl2 --libs`.gsub("\n", "")})#-Iminifb/include -Iminifb/src minifb/src/*.c minifb/src/macosx/*.m -Iinclude -framework Metal -framework MetalKit -framework AppKit)
end

# Generate documentation
cmd :docs do
  sh silent %(rm -rf docs/html)
  sh silent %(rm -rf docs/latex)
  Dir.chdir("docs") do
    sh %(doxygen argon.doxygen)
  end
end

$beaver.end

