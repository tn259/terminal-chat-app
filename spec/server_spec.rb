require_relative '../bin/server'
require 'spec_helper'
require_relative '../lib/chat-client-handler'

describe Server do

	s = Server.new "127.0.0.1", 3000

	describe "correct initial values" do
		it "has port == 3000" do
			expect(s.port).to eq(3000)
		end
		it "has ip == 127.0.0.1" do
			expect(s.ip).to eq("127.0.0.1")
		end
		it "has a TCPServer" do 
			expect(s.server).to be_a TCPServer
		end
	end
	
	describe "instance variables not reassigned" do
		it "@port cannot be reassigned" do
			expect{s.port = 2000}.to raise_error NoMethodError	
		end
		it "@ip cannot be reassigned" do 
			expect{s.ip = ""}.to raise_error NoMethodError
		end
		it "@TCPServer cannot be reassigned" do
			expect{s.server = TCPServer.new 2000}.to raise_error NoMethodError	
		end
	end

	describe "instance variable internals immutable" do
		it "has immutable port" do
			expect(s.port.frozen?).to eq(true)
		end
		it "has immutable ip" do
			expect(s.ip.frozen?).to eq(true)
		end
	end
	
	context "#run" do
			socket = new TCPSocket.new s.ip, s.port
			so = s.server.accept
		it "connects to socket" do
			expect{ChatClientHandler.new(so).run}.to_not raise_error SocketError  
		end

			socket1 = TCPSocket.new s.ip, s.port
			socket2 = TCPSocket.new s.ip, s.port
		it "receives and sends messages" do
			output = ""
			t1 = Thread.new {s.run}
			sleep 3
			t2 = Thread.new {socket1.puts "Test\n"}
			sleep 3 
			t3 = Thread.new {output += socket2.gets}
			t3.join 
			expect(output).to match /Test/
		end
	end	
end 
