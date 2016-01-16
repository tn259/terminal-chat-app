require_relative '../bin/server'
require 'spec_helper'

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
	
	#How should I test server#run?	
	#context "#run" do
	#		socket = new TCPSocket.new s.ip, s.port
	#	it "connects to socket and transfers messages" do
	#		t1 =    Thread.start(s.server.accept) do |client|
        #                		client.puts "Welcome to the Terminal-Chat-App"
        #                	end
	#		t2 =    Thread.new {
	#				output = socket.gets
	#				expect(output).matches /Welcome to the Terminal-Chat-App/
	#				socket.close
	#			}
	#		t1.join
	#		t2.join
	#	end
	#end	
end 
