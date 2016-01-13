require_relative '../src/server'
require 'spec_helper'

describe Server do

	s = Server.new "127.0.0.1", 3000

	it "has port == 3000" do
		expect(s.port).to eq(3000)
	end

	it "has ip == 127.0.0.1" do
		s.ip.should == "127.0.0.1"
	end
	
#The following made me understand that attr_readable means that the instance variables can be modified internally but you cannot point them to different objects
#Must remember that everything in ruby is treated as an object

	it "has immutable port" do
		s.port << 2000
		s.port.should == 3000
	end

#fails because the querty string can be modified internally
#TODO make copies of instance variables with .dup
	it "has immutable ip" do
		s.ip << "qwerty"
		expect(s.ip).to eq("127.0.0.1")
	end


end
