require 'socket'

class ChatClientHandler
	attr_reader :client

	def initialize(client)
		@client = client
	end

	def run
		begin
			@client.puts "Welcome to the Terminal-Chat-App"
			@client.close
		rescue	SocketError =>  e
			puts e.message
		end
	end
end
