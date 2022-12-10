#include <algorithm>
#include <cassert>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>
#include <vector>


class NumberedFile
{
public:
  NumberedFile(std::string dest_path, std::string buffer_path) : dest_path(dest_path), buffer_path(buffer_path) {}

  std::string get_buffer_path()
  {
    return buffer_path;
  }

  void flush()
  {
    assert(system(("mv " + buffer_path + " " + dest_path).c_str()) == 0);
  }

private:
  std::string dest_path;
  std::string buffer_path;
};

class NumberedFiles
{
public:
  NumberedFiles(std::string root_dir, std::string tmp_dir, int files_per_dir = 1000) : root_dir(root_dir), tmp_dir(tmp_dir), files_per_dir(files_per_dir)
  {
    assert(system(("mkdir -p " + root_dir).c_str()) != -1);
    assert(system(("mkdir -p " + tmp_dir).c_str()) != -1);
    
    next_id = get_max_id(root_dir) + 1;
  }

  NumberedFile create()
  {
    std::lock_guard<std::mutex> lock(lock_mutex);

    int id = get_and_increase_next_id();

    std::stringstream tmp_ss;
    tmp_ss << tmp_dir << "/" << id;

    return NumberedFile(get_file_path(id), tmp_ss.str());
  }

  std::vector<std::string> get_latest_file_paths(int limit)
  {
    std::vector<std::string> res;

    std::vector<int> dirs = list_dirs_or_files(root_dir, true);
    std::sort(dirs.begin(), dirs.end());

    while(!dirs.empty()){
      int dir_id = dirs.back();
      dirs.pop_back();

      std::vector<int> files = list_dirs_or_files(get_dir_path(root_dir, dir_id), false);
      std::sort(files.begin(), files.end());

      while(!files.empty()){
        int id = files.back();
        files.pop_back();

        res.push_back(get_file_path(id));
        if(res.size() == limit) return res;
      }
    }

    return res;
  }

private:

  std::string get_file_path(int id)
  {
    std::stringstream ss;
    ss << get_or_create_dir_path(id) << "/" << id;
    return ss.str();
  }

  std::string get_or_create_dir_path(int id)
  {
    std::stringstream ss;
    ss << root_dir << "/" << id / files_per_dir;
    std::string path = ss.str();
    if(!has_dir(path)){
      assert(system(("mkdir -p " + path).c_str()) != -1);
      usleep(100000);
      assert(has_dir(path));
    } 
    return path;
  }

  int get_and_increase_next_id()
  {
    ++next_id;
    return next_id - 1;
  }

  static int get_max_id(std::string root_dir)
  {
    std::vector<int> dirs = list_dirs_or_files(root_dir, true);
    std::sort(dirs.begin(), dirs.end());

    while(!dirs.empty()){
      int dir_id = dirs.back();

      dirs.pop_back();

      std::vector<int> files = list_dirs_or_files(get_dir_path(root_dir, dir_id), false);
      std::sort(files.begin(), files.end());

      if(files.empty()) continue;
      return files.back();
    }
    return 0;
  }

  static bool has_dir(std::string path)
  {
    class stat st;
    if(stat(path.c_str(), &st) != 0) return false;
    if(st.st_mode & S_IFDIR) return true;
    return false;
  }

  static std::string get_dir_path(std::string root_dir, int dir_id)
  {
    std::stringstream ss;
    ss << root_dir << "/" << dir_id;
    return ss.str();
  }

  static std::vector<int> list_dirs_or_files(std::string target_dir, bool should_dir)
  {
    std::vector<int> res;

    DIR *dir = opendir(target_dir.c_str());
    for(dirent *ent; (ent = readdir(dir)) != NULL;){
      std::string name = ent->d_name;
      if(name.size() == 0) continue;
      if(name[0] == '.') continue;

      class stat st;
      if(stat((target_dir + "/" + name).c_str(), &st) == -1) continue;

      bool is_dir = (st.st_mode & S_IFDIR) != 0;
      if(is_dir != should_dir) continue;

      res.push_back(std::stoi(name));
    }
    closedir(dir);
    
    return res;
  }

  std::string root_dir;
  std::string tmp_dir;
  int next_id;
  int files_per_dir;

  std::mutex lock_mutex;
};




