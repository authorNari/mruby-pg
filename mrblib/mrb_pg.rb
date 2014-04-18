module PG
  class Connection
    CONNECT_ARGUMENT_ORDER = %w[host port options tty dbname user password]

    def self.quote_connstr(value)
      return "'" + value.to_s.gsub("'") {|m| '\\' + m } + "'"
    end

    def self.parse_connect_args(*args)
      return '' if args.empty?

      connopts = []

      # Handle an options hash first
      if args.last.is_a?(Hash)
        opthash = args.pop
        opthash.each do |key, val|
          connopts.push("#{key}=#{PG::Connection.quote_connstr(val)}")
        end
      end

      # Option string style
      if args.length == 1 && args.first.to_s.index('=')
        connopts.unshift(args.first)
      else
        args.each_with_index do |val, i|
          next unless val # Skip nil placeholders

          key = CONNECT_ARGUMENT_ORDER[i] or
            raise ArgumentError, "Extra positional parameter %d: %p" % [ i+1, val ]
          connopts.push("#{key}=#{PG::Connection.quote_connstr(val.to_s)}")
        end
      end

      return connopts.join(' ')
    end
  end
end
