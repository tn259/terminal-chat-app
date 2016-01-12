# server.rb
#
#Server has a TCPServer which accepts TCPSockets
# 
#
require 'socket'

class Server
	def initialize(ip, port)
		@server = TCPServer.open(ip, port)
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

server = Server.new "127.0.0.1", 3000
server.run
