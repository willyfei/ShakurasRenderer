#pragma once
#include <vector>
#include <string>
#include <memory>
#include <array>


#define SHAKURAS_BEGIN namespace shakuras {
#define SHAKURAS_END }


SHAKURAS_BEGIN;


#define SHAKURAS_SHARED_PTR(C) typedef std::shared_ptr<C> C##Ptr


template<class T>
void unstable_erase(std::vector<T>& vec, size_t pos, size_t count) {
	size_t len = vec.size();
	size_t bpos = len - count;
	std::swap_ranges(vec.begin() + pos, vec.begin() + pos + count, vec.begin() + bpos);
	vec.erase(vec.begin() + bpos, vec.end());
}


typedef void(*pfnprogress)(int v);//0~100


//判断特定开头结尾
bool startwith(const std::string& str, const std::string& s);
bool endswith(const std::string& str, const std::string& e);


//去除开头
std::string& rmstart(std::string& str, std::string s);
std::string rmstartcopy(std::string str, std::string s);
std::string& trimstart(std::string& str, char c);
//去除结尾
std::string& rmends(std::string& str, std::string e);
std::string rmendscopy(std::string str, std::string e);
std::string& trimends(std::string& str, char c);


//按最后一个斜线分开，用于分离目录和文件
std::array<std::string, 2> pathsplit(std::string path);
//组合
std::string pathjoin(std::string dn, std::string fn);


//按最后一个.分开
std::array<std::string, 2> dotsplit(std::string path);
//组合
std::string dotjoin(std::string fn, std::string ext);


//存在文件或文件夹
bool pathexist(std::string path);
bool isfile(std::string path);
bool isdir(std::string path);


//按路径创建文件夹
void makedirs(std::string path);
//递归删除文件夹子树
void removedirs(std::string dn);


//复制文件
void copyfile(std::string src, std::string dest);
//递归复制文件夹
void copydir(std::string src, std::string dest, pfnprogress pfn = NULL);


//列举文件夹下的文件全路径
void getfiles(std::string dn, std::string filter, std::vector<std::string>& fps);
//递归列举文件夹下的文件全路径
void getfilesrec(std::string dn, std::string filter, std::vector<std::string>& fps);
//列举文件夹下的文件夹全路径
void getfolders(std::string dn, std::vector<std::string>& fps);


//转小写且‘\\’->'/'
void makestdpath(std::string& fp);


//取相对路径
std::string relpath(std::string prefix, std::string fp);


SHAKURAS_END;