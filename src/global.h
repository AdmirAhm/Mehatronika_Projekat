#pragma once
#include "td/String.h"

static td::String _message;
static td::String _message1;
bool close_server = false;
bool novi = false;
std::mutex _message_mutex;

void setMessage(td::String message) {
	_message_mutex.lock();
	_message = message;
	novi = true;
	_message_mutex.unlock();
}

void setMessage1(td::String message) {
	_message_mutex.lock();
	_message1 = message;
	//novi = true;
	_message_mutex.unlock();
}

td::String getMessage() {
	_message_mutex.lock();
	td::String message="";
	if(novi) message = _message;
	_message_mutex.unlock();
	return message;
}

td::String getMessage1() {
	_message_mutex.lock();
	td::String message="";
	if(novi) message = _message1;
	_message_mutex.unlock();
	return message;
}