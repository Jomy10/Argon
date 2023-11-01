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

cmd :test do
  call :build
  sh %(clang #{CFLAGS} -Iminifb/include -Iminifb/src test.c out/obj/*.o minifb/src/*.c minifb/src/macosx/*.m -Iinclude -framework Metal -framework MetalKit -framework AppKit)
end

$beaver.end

