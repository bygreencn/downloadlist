#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <json/json.h>
#include <cpr/cpr.h>

int main(int argc, char** argv) {
    std::string conf, dir;
    bool checkOption = false;
    if (argc == 3) {
        conf = argv[1];
        dir = argv[2];
        std::string confOption("--conf="), dirOption("--dir=");
        size_t p = conf.find(confOption), q = dir.find(dirOption);
        if (p == 0 && q == 0) {
            conf = conf.substr(confOption.size());
            dir = dir.substr(dirOption.size());
            if (!(conf.empty() || dir.empty())) {
                checkOption = true;
            }
        }
    }

    if (!checkOption) {
        std::cout << "options error." << std::endl;
        std::cout << "example:downloadlist --conf=downloadlist.json --dir=download" << std::endl;
        return -1;
    }

    struct stat sb;
    if (stat(dir.c_str(), &sb) && mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO)) {
        std::cout << "dir is not exists.create dir is not successful." << std::endl;
        return -1;
    }

    std::ifstream in;
    in.open(conf.c_str());
    if (!in.is_open()) {
        std::cout << (conf + " not found.") << std::endl;
        return -1;
    }

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(in, root)) {
        std::cout << "conf parse failed." << std::endl;
        in.close();
        return -1;
    }
    in.close();

    auto downfun = [&](const std::string& name, const std::string & url, time_t timeout) {
        if (!name.empty()&&!url.empty()) {
            cpr::Response res = cpr::Get(cpr::Url{url}, cpr::Timeout(timeout * 1000));
            if (res.status_code == 200) {
                std::ofstream out(dir + "/" + name, std::ios::out | std::ios::binary | std::ios::trunc);
                out << res.text;
            } else {
                std::cout << url << " not download " << std::endl;
            }
            std::cout << ("download " + name + " take ") << res.elapsed << " seconds " << std::endl;
        }
    };

    std::string name, url;
    time_t timeout = 60;
    std::vector<std::shared_ptr<std::thread> > thList;
    for (int i = 0; i < root.size(); ++i) {
        if (!root[i]["name"].isNull()&&!root[i]["url"].isNull()&&!root[i]["timeout"].isNull()) {
            name = root[i]["name"].asString();
            url = root[i]["url"].asString();
            timeout = root[i]["timeout"].asUInt();
            std::shared_ptr<std::thread> th(new std::thread(downfun, name, url, timeout));
            thList.push_back(th);
        }
    }
    for (auto & item : thList) {
        item->join();
    }
    return 0;
}

