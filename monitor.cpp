#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <ctime>
#include <openssl/sha.h>

using namespace std;
namespace fs = std::filesystem;

string current_time()
{
    time_t now=time(0);
    char *dt=ctime(&now);
    string t(dt);
    t.pop_back();
    return t;
}

void log_event(string msg)
{
    ofstream log("security_log.txt",ios::app);
    log<<"["<<current_time()<<"] "<<msg<<"\n";
}

string sha256(const string &filename)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    ifstream file(filename, ios::binary);
    char buf[4096];

    while(file.read(buf,sizeof(buf)))
        SHA256_Update(&sha256,buf,file.gcount());

    SHA256_Update(&sha256,buf,file.gcount());
    SHA256_Final(hash,&sha256);

    string output="";
    char tmp[3];
    for(int i=0;i<SHA256_DIGEST_LENGTH;i++)
    {
        sprintf(tmp,"%02x",hash[i]);
        output+=tmp;
    }
    return output;
}

map<string,string> load_database()
{
    map<string,string> db;
    ifstream in("hashes.txt");
    string path,hash;

    while(in>>path>>hash)
        db[path]=hash;

    return db;
}

void save_database(map<string,string> &db)
{
    ofstream out("hashes.txt");
    for(auto &p:db)
        out<<p.first<<" "<<p.second<<"\n";
}

int main()
{
    string directory="testdir";

    map<string,string> old_db = load_database();
    map<string,string> new_db;

    for(auto &p: fs::recursive_directory_iterator(directory))
    {
        if(!fs::is_regular_file(p)) continue;

        string path=p.path();
        string hash=sha256(path);
        new_db[path]=hash;

        if(old_db.count(path))
        {
            if(old_db[path]!=hash)
            {
                cout<<"Modified: "<<path<<"\n";
                log_event("Modified: "+path);
            }
        }
        else
        {
            cout<<"New file: "<<path<<"\n";
            log_event("New file: "+path);
        }
    }

    for(auto &p:old_db)
    {
        if(!new_db.count(p.first))
        {
            cout<<"Deleted: "<<p.first<<"\n";
            log_event("Deleted: "+p.first);
        }
    }

    save_database(new_db);
}
