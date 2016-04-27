#include <iostream>
#include <fstream>
#include <thread>
#include <json/json.h>
#include <cpr/cpr.h>

int main(int argc, char** argv) {
	const std::string conf("download/downloadlist.json");
	std::ifstream in;
	in.open(conf.c_str());
	if (!in.is_open()) {
		std::cout << (conf + " not found") << std::endl;
		return -1;
	}
	Json::Value root;
	Json::Reader reader;
	reader.parse(in, root);
	in.close();
	std::string name, url;
	auto downfun = [](const std::string& name, const std::string & url) {
		std::cout << "thread start " << name << std::endl;
		cpr::Response res = cpr::Get(cpr::Url{url});
		if (res.status_code == 200) {
			std::ofstream out("download/" + name, std::ios::out | std::ios::binary | std::ios::trunc);
			out << res.text;
		} else {
			std::cout << name << " not download " << std::endl;
		}
		std::cout << "thread stop " << name << std::endl;
	};

	for (int i = 0; i < root.size(); ++i) {
		name = root[i]["name"].asString();
		url = root[i]["url"].asString();
		std::shared_ptr<std::thread> th(new std::thread(downfun, name, url));
		th->join();
	}
	return 0;
}

