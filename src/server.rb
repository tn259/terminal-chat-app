# server.rb
#
#Server has a TCPServer which accepts TCPSockets
# 
#
require 'socket'

class Server
	#allow reading of ip and port as an exercise in testing for now
	attr_reader :ip
	attr_reader :port
	def initialize(ip, port)
		@ip = ip
		@port = port
		@server = TCPServer.new(ip, port)
	end
#Listen for multiple clients and close connection after welcoming each one
	def run 
	loop {
		print "Awaiting new client..."	
		Thread.start(@server.accept) do |client|
			client.puts "Welcome to the Terminal-Chat-App"
			client.puts "I'm afraid I'll have to say by for now"
			client.close
		end
	}
	end
end

#server = Server.new "127.0.0.1", 3000 
#server.run
