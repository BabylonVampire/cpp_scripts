#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

void ceasar(string &str, string key) {
	int offset = stoi(key);
	int length = str.length();

	for (size_t i = 0; i < length; ++i) {
		str[i] = (str[i] + offset) % 256;
	}
}

void vernam(string &str, string key) {
	int key_length = key.length();
	int length = str.length();

	for (size_t i = 0; i < length; ++i) {
		if (str[i] != ' ') {
			str[i] = str[i] ^ key[i % key_length];
		}
	}
}

void alphabet_reverse(string &str, string key) {
	int length = str.length();

	for (size_t i = 0; i < length; ++i) {
		if (str[i] != ' ') {
			str[i] = 255 - str[i];
		}
	}
}

void decrypt_ceasar(string &str, string key) {
	int length = str.length();
	int offset = stoi(key);

	for (size_t i = 0; i < length; ++i) {
		if (str[i] - offset < 0) str[i] = str[i] - offset + 255;
		else str[i] = (str[i] - offset) % 255;
	}
}

void decrypt_vernam(string &str, string key) {
	int key_length = key.length();
	int length = str.length();

	for (size_t i = 0; i < length; ++i) {
		if (str[i] != ' ') {
			str[i] = str[i] ^ key[i % key_length];
		}
	}
}

void encrypt(string combination, void(**cipher_funks)(string&, string), string name_of_file) {
	int index, length;
	bool writing_key = false, first_stroke = true;
	ifstream target_file;
	ofstream new_file;
	string lockal_key, text_of_stroke, key, public_key = "";
	length = combination.length();
	string* keys = new string[length];

	target_file.open(name_of_file + ".txt");
	new_file.open(name_of_file + "_encrypted.txt");

	while (getline(target_file, text_of_stroke)) {
		if (first_stroke) {
			for (size_t i = 0; i < length; ++i) {
				key = "";
				index = combination[i] - '0';
				switch (index) {
				case 1:
					cout << "[+] Enter the size of offset: ";
					cin >> key;
					keys[i] = key;
					break;
				case 2:
					cout << "[+] Enter secret key for vernam cipher: ";
					cin >> key;
					keys[i] = key;
					break;
				case 3:
					cout << "[+] Alphabet has been reversed." << endl;
					break;
				default:
					cout << "[-] Error: Could not find function!" << endl;
					system("pause");
					exit(1);
				}
				cipher_funks[index - 1](text_of_stroke, key);
				public_key += to_string(index - 1) + "(" + key + ")";
				first_stroke = false;
			}
		}
		else {
			for (size_t i = 0; i < length; ++i) {
				index = combination[i] - '0';
				key = keys[i];
				if (index > 4) {
					cout << "[-] Error: Could not find function!" << endl;
					system("pause");
					exit(1);
				}
				cipher_funks[index - 1](text_of_stroke, key);
			}
		}
		new_file << text_of_stroke << "\n";
	}

	cout << "[+] Your key-code is: " + public_key << endl;

	delete[] keys;
	new_file.close();
}

void decrypt(string combination, void(**decipher_funks)(string&, string), string name_of_file) {
	string lockal_key = "", text_of_stroke;
	bool writing_key = false;
	int length = combination.length();
	ifstream target_file;
	ofstream new_file;
	vector<string> keys;
	vector<int> funks;

	for (size_t i = 0; i < length; ++i) {
		if (combination[i] == '(') {
			writing_key = true;
		}
		else if (combination[i] == ')') {
			writing_key = false;
			keys.push_back(lockal_key);
			lockal_key = "";
		}
		else if (writing_key) lockal_key += combination[i];
		else funks.push_back(combination[i]);
	}

	target_file.open(name_of_file + ".txt");
	new_file.open(name_of_file + "_decrypted.txt");
	length = funks.size();

	while (getline(target_file, text_of_stroke)) {
		for (int i = 0; i < length; ++i) {
			decipher_funks[funks[length - i - 1] - '0'](text_of_stroke, keys[length - i - 1]);
		}
		new_file << text_of_stroke + "\n";
	}
}

void information() {
	cout << "Welcome to the text cipherer!\nHere you can protect your information and decrypt one, that alredy encrypted!\nHave fun :)" << endl;
	cout << "1) Caesar cipher" << endl;
	cout << "2) Vernam cipher" << endl;
	cout << "3) reverse alphabet" << endl;
	cout << "Commands:" << endl;
	cout << "1. 'encrypt' and combination of numbers of ciphers to encrypt text." << endl;
	cout << "2. 'decrypt' and combination of numbers of ciphers to decrypt text." << endl;
}

void main() {
	string input_command, name_of_file, input_combination = "";

	information();

	cout << "[+] Enter name of text file, you want to encrypt/decrypt: ";
	cin >> name_of_file;

	cout << "[+] command: ";
	cin >> input_command >> input_combination;

	void(**cipher_funks)(string&, string) = new (void(*[3])(string&, string));
	cipher_funks[0] = ceasar;
	cipher_funks[1] = vernam;
	cipher_funks[2] = alphabet_reverse;

	void(**decrypt_funks)(string&, string) = new (void(*[3])(string&, string));
	decrypt_funks[0] = decrypt_ceasar;
	decrypt_funks[1] = decrypt_vernam;
	decrypt_funks[2] = alphabet_reverse;

	if (input_command == "encrypt" && input_command != "") {
		encrypt(input_combination, cipher_funks, name_of_file);
	}
	else if (input_command == "decrypt" && input_command != "") {
		decrypt(input_combination, decrypt_funks, name_of_file);
	}
	else {
		cout << "[-] Please, enter correct command!" << endl;
		system("pause");
		exit(1);
	}

	cout << "[+] Programm finished succesfuly!" << endl;

	delete[] cipher_funks;
	delete[] decrypt_funks;
}
