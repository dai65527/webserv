require 'net/http'

class WebservTestCase
  attr_accessor :casename, :request, :expectedBody, :expectedResponseHeader, :expectedResponseHeaderExitance, :expectedCode

  @@cases = []

  def initialize(casename, host, port, path, method)
    @casename = casename

    # parse uri
    @uri = URI.parse("http://" + host + ":#{port}" + path)

    # parse method
    case method
    when "GET" then
      @request = Net::HTTP::Get.new(path)
    when "HEAD" then
      @request = Net::HTTP::Head.new(path)
    when "POST" then
      @request = Net::HTTP::Post.new(path)
    when "PUT" then
      @request = Net::HTTP::Put.new(path)
    when "DELETE" then
      @request = Net::HTTP::Delete.new(path)
    when "CONNECT" then
      @request = Net::HTTP::Connect.new(path)
    when "OPTIONS" then
      @request = Net::HTTP::Options.new(path)
    when "TRACE" then
      @request = Net::HTTP::Trace.new(path)
    else
      puts "[warning] unknown method #{method}, MKCOL was set instead"
      @request = Net::Mkcol.new(path)
    end

    # expected results
    @expectedCode = "200"
    @expectedResponseHeader = {"dummy" => "dummy"}
    @expectedResponseHeaderExitance = ["dummy"]
    @expectedBody = "dummy"
  end

  def add
    @@cases.push self
  end

  def self.execAll
    ret = 0
    @@cases.each_with_index do |testcase, index|
      puts "case #{index + 1}: #{testcase.casename}"
      begin
        testcase.exec
        puts "    OK :)"
      rescue => e
        puts e
        ret = 1
      end
    end
    ret
  end

  def exec
    response = Net::HTTP.start(@uri.host, @uri.port) { |http|
      http.request(request)
    }
    assert_eq(response.code, @expectedCode)
    if expectedBody != "dummy"
      assert_eq(response.body, @expectedBody)
    end
    @expectedResponseHeader.each{|key, value|
      if key != "dummy"
        assert_eq(value, response[key])
      end
    }
    @expectedResponseHeaderExitance.each{ |key|
      if key != "dummy"
        assert_true(response[key])
      end
    }
  end

  private

  def assert_eq(a, b)
    if a != b
      raise StandardError.new("should be equal: \"#{a}\": \"#{b}\"")
    end
  end

  def assert_true(a)
    if !a
      raise StandardError.new("should be true")
    end
  end
end
