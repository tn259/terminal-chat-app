# server.rb
#
#Server has a TCPServer which accepts TCPSockets
# 
#

# TODO: Move this file into a bin directory in the project. Ruby code usually
#       lives in a lib directory with the executables in a bin directory. Worth
#       google-ing or looking at the source of a gem on git-hub to see what I mean.

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
			# TODO: consider what happens here if one of these puts fails...
			client.puts "Welcome to the Terminal-Chat-App"
			client.puts "I'm afraid I'll have to say by for now"
			client.close
		end
	}
	end
end

# This is a useful construct which says "if the user executed this file". This
# stops the code being run if this file is 'required' by another ruby script.
if __FILE__ == $0
	server = Server.new "127.0.0.1", 3000 
	server.run
end
