MRuby::Gem::Specification.new('mruby-pg') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Narihiro Nakamura'
  spec.summary = 'postgresql mruby binding'

  spec.cc.include_paths << `pg_config --includedir`.chomp
  spec.linker.libraries << 'pq'
end
