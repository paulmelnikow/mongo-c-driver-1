#
# IMPORTANT
#
# This development branch depends on an unreleased development version of
# libbson.
#
# CocoaPods does not allow pods to depend on pods from git, so you need
# to pull them in from your Podfile instead:
# 
# pod 'libbson', :git => 'https://github.com/paulmelnikow/libbson', :branch => 'podspec-1'
# pod 'mongo-c-driver', :git => 'https://github.com/paulmelnikow/mongo-c-driver-1', :branch => 'podspec'
#
# Note these forks, which you need to use because the 10gen repositories
# do not contain podspecs at this time. These forks are unstable and
# are for development purposes only.
#

Pod::Spec.new do |s|
  s.name                 = "mongo-c-driver"
  s.version              = "2.0.0-dev"
  s.summary              = "A high-performance MongoDB driver for C."
  s.description          = <<-DESC
                           mongo-c-driver is a client library written in C for MongoDB.
                           DESC
  s.homepage             = "https://github.com/mongodb/mongo-c-driver"
  s.license              = { :type => "Apache License, Version 2.0", :file => "COPYING" }
  s.author               = "10gen"
  s.social_media_url     = "http://twitter.com/mongodb"
  s.source               = { :git => "https://github.com/mongodb/mongo-c-driver.git", :tag => "#{s.version}" }
  s.prepare_command      = './autogen.sh && ./configure --with-libbson=no'
  s.source_files         = "src/mongoc/*.{c,h}"
  s.header_mappings_dir  = "src"
  s.private_header_files = "src/mongoc/*-private.h"
  s.preserve_paths       = "src/mongoc/*.{def,defs}"
  s.compiler_flags       = "-DMONGOC_COMPILATION"
  s.requires_arc         = false
#  s.dependency             'libbson', '~> 1.1.0-rc0'
end
