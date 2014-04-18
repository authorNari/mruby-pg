assert('PG::Connection.new') do
  begin
    PG::Connection.new({host: "invalid", port: 90})
    break false
  rescue PG::Error
    true
  end
end
