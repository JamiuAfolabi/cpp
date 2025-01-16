 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include <memory>

using namespace std;

class StreamHandler: public EnableSharedFromThis<StreamHandler>
{
  private:
	ifstream file;
	bool fileOpened;
	string buffer;
	vector<string> fileRows;
	int rowindex;
	vector<size_t> rowStarts;
	enum class State {IN_QUOTE, NOT_IN_QUOTE};
	string row;
	State bufferState;
	char lastChar read;
  public:
    StreamHandler(const string& fileName): fileOpened{false}, rowindex{0}, bufferState{State::NOT_IN_QUOTE}, lastChar(' '){
	    openFile(fileName);
    }

	~StreamHandler() {
		if(file.is_open()) {
			file.close();
			fileOpened = false;
		}
	}
 
	StreamHandler(const StreamHandler&) = delete;
	StreamHandler& operator=(const StreamHandler&) = delete;

	StreamHandler(StreamHandler&& other) noexcept: file(std::move(other.file)), 
		fileOpened(other.fileOpened), 
		buffer(std::move(other.buffer)), 
		fileRows(std::move(other.fileRows)), 
		rowindex(other.rowindex), 
		rowStarts(std::move(other.rowStarts)),
		row(std::move(other.row)),
		bufferState(other.bufferState),
		lastChar(other.lastChar){
			other.fileOpened = false;
		}

	StreamHandler& operator=(StreamHandler&& other) noexcept {
		file.close();
		fileOpened = false;
		this->~StreamHandler();
		*this = std::move(other);
		return *this;
	}
 
	//Custom utf-8 decoding functionality
	std::string readUTF8Char() {
		std::string utf8char;
		int utf8NumberOfBytes = 0;
		unsigned int charBuff = (unsigned char)lastChar;
		if ((charBuff & 0x80) == 0) {
			utf8NumberOfBytes = 1;
		} else if ((charBuff & 0xE0) == 0xC0) {
			utf8NumberOfBytes = 2;
		} else if ((charBuff & 0xF0) == 0xE0) {
			utf8NumberOfBytes = 3;
		} else if ((charBuff & 0xF8) == 0xF0) {
			utf8NumberOfBytes = 4;
		}
		if (utf8NumberOfBytes == 1) {
			utf8char = {lastChar};
		} else {
			while (file.is_open() && utf8char.length() < utf8NumberOfBytes) {
				file >> lastChar;
				if (file.fail()) {
					throw std::runtime_error("Error reading file due to end of file or unexpected character.");
				}
				utf8char += lastChar;
			}
			if(utf8char.length() != utf8NumberOfBytes){
				throw std::runtime_error("Unexpected byte sequence when parsing UTF-8 characters.");
			}
		}
		return utf8char;
	}

	// Handle quoted fields with newlines
	string readField() {
		string field;
		bool quotedField = lastChar == '"';
		bufferState = quotedField ? State::IN_QUOTE : State::NOT_IN_QUOTE;