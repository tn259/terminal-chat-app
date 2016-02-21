require 'socket'

class ChatClientHandler
	attr_reader :client
	attr_reader :message

	def initialize(client)
		@client = client
	end

	def run
		begin
			@client.puts "test\n"
			@client.close
		rescue	SocketError =>  e
			puts e.message
		end
	end
end
