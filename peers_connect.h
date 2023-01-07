#include "tracker_response.h"
#include <string>
bool handshake(Peer peer, std::string info_hash, std::string client_id);
void receiver(std::vector<char> received_message);