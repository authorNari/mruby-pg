# mruby-pg

Mini PostgreSQL binding for mruby.

## Usage

```ruby
# Establish connectoin
@conn = PG::Connection.new(port: 5432, dbname: "test")

# Send SQL
@conn.exec("create table students (id int, name varchar(256))")
@conn.exec("insert into students (id, name) VALUES (1, 'bob')")
@conn.exec("select * from students") do |result|
  puts result["id"]   # => "1"
  puts result["name"] # => "bob"
end

# Transaction
@conn.transaction do
  @conn.exec("insert into students (id, name) VALUES (2, 'jon')")
end
```

## Thanks

This mgem is mini ruby-pg. Thank you, ruby-pg developers.
https://bitbucket.org/ged/ruby-pg/wiki/Home
