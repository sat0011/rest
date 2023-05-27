#include <iostream>
#include <vector>
#include <fstream>
#include <SDL.h>
#include <SDL_net.h>

using namespace std;

#pragma warning(disable : 4996)

#undef main
int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	SDLNet_SocketSet set;
	TCPsocket local, remote;
	IPaddress localIP, remoteIP;

	char request[1024];
	int requestLenght = 0;
	string response;

	int parsePointer;
	int comparisonPointer;

	string requestMethod;
	string requestType;
	string requestID_str;
	int requestID;
	int connectionStatus;

	set = SDLNet_AllocSocketSet(1);

	SDLNet_ResolveHost(&localIP, NULL, 8080);
	local = SDLNet_TCP_Open(&localIP);
	/*
	remote = NULL;
	printf("waiting for a connection\n");
	while (!remote) {
		remote = SDLNet_TCP_Accept(local);
	}
	remoteIP = *SDLNet_TCP_GetPeerAddress(remote);
	SDLNet_TCP_AddSocket(set, remote);
	printf("connection received\n");
	printf("remote IP: %s \n", SDLNet_ResolveIP(&remoteIP));
	*/
	/*
	int connectionStatus = 0;
	while (SDLNet_CheckSockets(set, 0) >= 1) {
		connectionStatus = SDLNet_TCP_Recv(remote, &request[requestLenght], sizeof(char));
		printf("received %d bytes of data\n", connectionStatus);
		requestLenght++;
		printf("request lenght = %d\n", requestLenght);
	}
	request[requestLenght] = '\0';
	printf("received request, lenght = %d\n", requestLenght);
	printf("request content:\n%s \n", request);
	*/
	//------------------------------------------------------------------------------------------//
	/*
	char postContents[] = "example post";
	int postDate = 0; // 
	int* posters[10];
	int* replies[10];

	char replyContents[][256] = {"hello world", "reply one"};
	int lenght[] = { 0, 1 };
	int replyDate[] = { 0, 2 };

	char username[][32] = { "test user one", "test user two"};
	int postCount[] = { 0, 1 };
	char pass[][32] = { "qwerty", "1234" };
	*/

	vector<string> postContents;
	vector<int> postDate;
	vector<vector<int>> posters;
	vector<vector<int>> replies;

	vector<string> replyContents;
	vector<int> replyDate;
	vector<int> replyAuthor;
	
	vector<string> username;
	vector<int> postCount;
	vector<string> pass;

	const char* keywords[11];
	keywords[0] = "\r\n\r\n"; keywords[1] = "postContents"; keywords[2] = "postDate"; keywords[3] = "posters"; keywords[4] = "replyContents";
	keywords[5] = "replyDate"; keywords[6] = "replyAuthor"; keywords[7] = "username"; keywords[8] = "pass";

	char buffer[256];

	postContents.push_back("test post one");
	postDate.push_back(0);
	posters.resize(postContents.size());
	replies.resize(postContents.size());
	posters[0].push_back(0);
	replies[0].push_back(0);

	postContents.push_back("post test two");
	postDate.push_back(1);
	posters.resize(postContents.size());
	replies.resize(postContents.size());
	posters[1].push_back(0);
	replies[1].push_back(0);

	replyContents.push_back("reply no1");
	replyDate.push_back(0);
	replyAuthor.push_back(0);

	username.push_back("username1");
	postCount.push_back(9);
	pass.push_back("qwerty");

	while (true) {

		remote = NULL;
		printf("waiting for a connection\n");
		while (!remote) {
			remote = SDLNet_TCP_Accept(local);
		}
		remoteIP = *SDLNet_TCP_GetPeerAddress(remote);
		SDLNet_TCP_AddSocket(set, remote);
		printf("connection received\n");
		printf("remote IP: %s \n", SDLNet_ResolveIP(&remoteIP));

		for (int i = 0; i < sizeof(request) / sizeof(char); i++) request[i] = NULL;
		requestLenght = 0;

		connectionStatus = 0;
		while (SDLNet_CheckSockets(set, 0) >= 1) {
			connectionStatus = SDLNet_TCP_Recv(remote, &request[requestLenght], sizeof(char));
			printf("received %d bytes of data\n", connectionStatus);
			requestLenght++;
			printf("request lenght = %d\n", requestLenght);
		}
		request[requestLenght] = '\0';
		printf("received request, lenght = %d\n", requestLenght);
		printf("request content:\n%s \n", request);

		parsePointer = 0;
		requestMethod.clear();
		requestType.clear();
		requestID_str.clear();
		while (request[parsePointer] != ' ') {
			requestMethod += request[parsePointer];
			parsePointer++;
		}
		parsePointer += 2;
		while (request[parsePointer] != ' ' && request[parsePointer] != '/') {
			requestType += request[parsePointer];
			parsePointer++;
		}
		if (request[parsePointer] != ' ') {
			parsePointer++;
			while (request[parsePointer] != ' ') {
				requestID_str += request[parsePointer];
				parsePointer++;
			}
			requestID = atoi(requestID_str.c_str());
		}
		else {
			requestID = 0;
		}

		switch (requestMethod[0]) {
		case 'G': {
			response += "HTTP/1.1 200 OK\r\n\r\n";
			switch (requestType[0]) {
			case 'p': { // post
				for (int i = requestID; i < postContents.size(); i++) {
					response += "post contents: \"";
					response += postContents[i];
					response += "\"\n";
					response += "post date: \"";
					_itoa(postDate[i], buffer, 10);
					response += buffer;
					response += "\"\n";

					for (int j = 0; j < posters[i].size(); j++) {
						response += "posters: \"";
						response += username[posters[i][j]];
						response += "\"\n";
					}

					for (int j = 0; j < replies[i].size() != NULL; j++) {
						response += "replies: \"";
						response += replyContents[replies[i][j]];
						response += "\"\n";
					}
					if (requestID != 0) break;
				}
				break;
			}
			case 'r': { // reply

				for (int i = requestID; i < replyContents.size(); i++) {
					response += "reply contents: \"";
					response += replyContents[i];
					response += "\"\n";
					response += "reply author: \"";
					response += username[replyAuthor[i]];
					response += "\'\n";
					response += "reply date: \"";
					_itoa(replyDate[i], buffer, 10);
					response += buffer;
					response += "\'\n";
					if (requestID != 0) break;
				}
				break;
			}
			case 'u': { // user

				for (int i = requestID; i < username.size(); i++) {
					response += "username: \'";
					response += username[i] += "\'\n";
					response += "post count: ";
					_itoa(postCount[i], buffer, 10);
					response += buffer;
					response += "\n";
					response += "password: \'";
					response += pass[i] += "\'\n";
					if (requestID != 0) break;
				}
				break;
			}
			default: {
				response += "HTTP/1.1 404 NOT FOUND\r\n\r\n";
				break;
			}
			}
			//response += "HTTP/1.1 200 OK\r\nContent-Length: ";
			//_itoa(responseBody.length(), buffer, 10);
			//response += buffer;
			//response += "\r\n\r\n";
			//response += responseBody;
			break;
		}
		case 'P': {

			while (parsePointer < requestLenght) {
				parsePointer++;
				for (int i = 0; request[parsePointer] == keywords[0][i]; i++) {
					parsePointer++;
					if (i == 3) goto crq;
				}
			}
			requestType[0] = 'q';
		crq:; // correct request
			response += "HTTP/1.1 200 OK\r\n\r\n";
			for (int i = 0; i < 255; i++) buffer[i] = NULL;
			switch (requestType[0]) {
			case 'p': {
				printf("request parsed correctly\n");
				printf("parse pointer at %d\n", parsePointer);
				while (parsePointer <= requestLenght) {
					for (comparisonPointer = 0; request[parsePointer] == keywords[1][comparisonPointer]; comparisonPointer++) {
						parsePointer++;
						if (comparisonPointer == 11) {
							printf("!!!!!!!!! found post content\n");
							parsePointer += 2;
							if (request[parsePointer] == ' ') parsePointer++;
							for (int j = 0; request[parsePointer] != '\n'; j++) {
								buffer[j] = request[parsePointer];
								parsePointer++;
							}
							postContents.push_back(buffer);
							printf("buffer = \n%s\n", buffer);
							comparisonPointer = -1;
						}
					}
					parsePointer -= comparisonPointer;
					for (comparisonPointer = 0; request[parsePointer] == keywords[2][comparisonPointer]; comparisonPointer++) {
						parsePointer++;
						if (comparisonPointer == 7) {
							for (int j = 0; j < 255; j++) buffer[j] = NULL;

							printf("!!!!!!!!! found post date\n");
							parsePointer += 2;
							if (request[parsePointer] == ' ') parsePointer++;
							for (int j = 0; request[parsePointer] != '\n' && parsePointer < requestLenght; j++) {
								buffer[j] = request[parsePointer];
								parsePointer++;
							}
							postDate.push_back(atoi(buffer));
							printf("buffer = \n%s\n", buffer);
							comparisonPointer = -1;
						}
					}
					parsePointer -= comparisonPointer;
					for (comparisonPointer = 0; request[parsePointer] == keywords[3][comparisonPointer]; comparisonPointer++) {
						parsePointer++;
						if (comparisonPointer == 6) {
							for (int j = 0; j < 255; j++) buffer[j] = NULL;

							printf("found posters\n");
							parsePointer += 2;
							for (int j = 0; request[parsePointer] != '\n' && parsePointer < requestLenght; j++) {
								buffer[j] = request[parsePointer];
								parsePointer++;
							}
							posters.resize(posters.size() + 1);
							posters.back().push_back(atoi(buffer));
							printf("buffer = \n%s\n", buffer);
							comparisonPointer = -1;
						}
					}
					parsePointer -= comparisonPointer;
					parsePointer++;

				}
				if (posters.size() != postContents.size() && posters.size() != postDate.size()) {
					posters.pop_back();
					postContents.pop_back();
					postDate.pop_back();
					response.clear();
					response = "HTTP/1. 400 BAD REQUEST\r\n\r\n";
				}

				break;
			}
			case 'r': {
				printf("request parsed correctly\n");
				printf("parse pointer at %d\n", parsePointer);
				while (parsePointer < requestLenght) {
					for (comparisonPointer = 0; request[parsePointer] == keywords[4][comparisonPointer]; comparisonPointer++) {
						parsePointer++;
						if (comparisonPointer == 12) {
							for (int j = 0; j < 255; j++) buffer[j] = NULL;

							printf("found reply contents\n");
							parsePointer += 2;
							for (int j = 0; request[parsePointer] != '\n'; j++) {
								buffer[j] = request[parsePointer];
								parsePointer++;
							}
							replyContents.push_back(buffer);
							printf("buffer = %s\n", buffer);
							comparisonPointer = -1;
						}
					}
					parsePointer -= comparisonPointer;
					for (comparisonPointer = 0; request[parsePointer] == keywords[5][comparisonPointer]; comparisonPointer++) {
						parsePointer++;
						if (comparisonPointer == 8) {
							for (int j = 0; j < 255; j++) buffer[j] = NULL;

							printf("found reply date\n");
							parsePointer += 2;
							for (int j = 0; request[parsePointer] != '\n' && parsePointer < requestLenght; j++) {
								buffer[j] = request[parsePointer];
								parsePointer++;
							}
							replyDate.push_back(atoi(buffer));
							printf("buffer = %s\n", buffer);
							comparisonPointer = -1;
						}
					}
					parsePointer -= comparisonPointer;
					for (comparisonPointer = 0; request[parsePointer] == keywords[6][comparisonPointer]; comparisonPointer++) {
						parsePointer++;
						if (comparisonPointer == 10) {
							for (int i = 0; i < 255; i++) buffer[i] = NULL;

							printf("found reply author\n");
							for (int i = 0; i < 255; i++) buffer[i] = NULL;

							parsePointer += 2;
							for (int j = 0; request[parsePointer] != '\n' && parsePointer < requestLenght; j++) {
								buffer[j] = request[parsePointer];
								parsePointer++;
							}
							replyAuthor.push_back(atoi(buffer));
							printf("buffer = %s\n", buffer);

							comparisonPointer = -1;
						}
					}
					parsePointer -= comparisonPointer;
					parsePointer++;
				}

				if (replyContents.size() != replyDate.size() && replyContents.size() != replyAuthor.size()) {
					replyContents.pop_back();
					replyDate.pop_back();
					replyAuthor.pop_back();
					response.clear();
					response = "HTTP/1.1 400 BAD REQUEST\r\n\r\n";
				}

				break;
			}
			case 'u': {
				while (parsePointer < requestLenght) {
					for (comparisonPointer = 0; request[parsePointer] == keywords[7][comparisonPointer]; comparisonPointer++) {
						parsePointer++;
						if (comparisonPointer == 7) {
							printf("found username\n");
							parsePointer += 2;
							for (int j = 0; request[parsePointer] != '\n'; j++) {
								buffer[j] = request[parsePointer];
								parsePointer++;
							}
							username.push_back(buffer);
							printf("buffer contents: %s\n", buffer);
							comparisonPointer = -1;
						}
					}
					parsePointer -= comparisonPointer;
					for (comparisonPointer = 0; request[parsePointer] == keywords[8][comparisonPointer]; comparisonPointer++) {
						parsePointer++;
						if (comparisonPointer == 3) {
							printf("found pass\n");
							parsePointer += 2;
							for (int j = 0; request[parsePointer] != '\n' && parsePointer < requestLenght; j++) {
								buffer[j] = request[parsePointer];
								parsePointer++;
							}
							pass.push_back(buffer);
							printf("buffer contents: %s\n", buffer);
							comparisonPointer = -1;
						}
					}
					parsePointer -= comparisonPointer;
					parsePointer++;
				}

				break;
			}

					if (username.size() != pass.size()) {
						username.pop_back();
						pass.pop_back();
						response.clear();
						response = "HTTP/1.1 400 BAD REQUEST\r\n\r\n";
					}
			}
			break;
		}
		case 'D': {
			//
			break;
		}
		default: {
			response += "HTTP/1.1 404 NOT FOUND\r\n\r\n";
		}
		}
		for (int i = 0; i < response.length(); i++) {
			SDLNet_TCP_Send(remote, &response[i], sizeof(char));
		}
		SDLNet_TCP_Close(remote);
		response.clear();
		printf("connection closed \n");
	}
	

}