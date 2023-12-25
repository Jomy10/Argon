#!/usr/bin/env ruby

require 'beaver'
require 'fileutils'
require 'colorize'

project = Project.new("Argon", build_dir: "out")
project.set_configs("Debug", "Release")
project.c_configs = {
  "Debug" => C::Configuration.new(cflags: ["-g"]),
  "Release" => C::Configuration.new(cflags: ["-O3"])
}

C::Library.new(
  name: "Argon",
  sources: "src/**/*.c",
  include: ["deps/olive.c", "include"]
)

C::Library.pkg_config("sdl2")

C::Executable.new(
  name: "test",
  sources: ["test.c"],
  dependencies: ["sdl2", "Argon"]
)

# Generate documentation
cmd :docs do
  _sh %(rm -rf docs/html)
  _sh %(rm -rf docs/latex)
  Dir.chdir("docs") do
    sh %(doxygen argon.doxygen)
  end
end

cmd :clean do
  sh %(rm -rf .beaver)
  sh %(rm -rf #{project.build_dir})
  sh %(rm -rf docs/html)
  sh %(rm -rf docs/latex)
end

$beaver.default_command = ""

