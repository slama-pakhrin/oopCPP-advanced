/****************************************************
Workshop 6

Name : Salon Lama Pakhrin
Email : slama-pakhrin@myseneca.ca
ID : 160535217
Date: Oct 28

I have done all the coding by myself and only
copied the code that my professor provided to 
complete my workshops and assignments.

****************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Filesystem.h"

using namespace std;
namespace sdds
{

    Filesystem::Filesystem(const std::string &fileName, const std::string &path)
    {

        std::ifstream inputFile(fileName);
        if (!inputFile)
        {
            throw invalid_argument("File not found: " + fileName);
        }
        else
        {
            m_root = new Directory(path);
            m_current = m_root;
            string line;
            while (getline(inputFile, line))
            {
                string filePath, fileContents;
                size_t delimiterPos = line.find("|");
                size_t pos1 = line.find_first_not_of(" ");
                size_t pos2 = line.find_last_not_of(" ", delimiterPos - 1);
                filePath = line.substr(pos1, pos2 - pos1 + 1);
                line.erase(0, delimiterPos + 1);
                pos1 = line.find_first_not_of(" ");
                line.erase(0, pos1);
                pos2 = line.find_last_not_of(" ");
                fileContents = line.substr(0, pos2 + 1);
                size_t pos = 0;
                vector<sdds::OpFlags> oflags;
                oflags.push_back(sdds::OpFlags::RECURSIVE);
                Directory *root = m_root;
                while ((pos = filePath.find("/")) != string::npos)
                {
                    if (!(m_root->find(filePath.substr(0, pos + 1), oflags)))
                    {
                        Directory *dir = new Directory(filePath.substr(0, pos + 1));
                        *root += dir;
                        root = dir;
                    }
                    else
                    {
                        root = dynamic_cast<Directory *>(m_root->find(filePath.substr(0, pos + 1), oflags));
                    }
                    filePath = filePath.substr(pos + 1);
                }

                if (delimiterPos != string::npos)
                {
                    if (!(m_root->find(filePath)))
                    {
                        File *file = new File(filePath, fileContents);
                        *root += file;
                    }
                }
            }
        }
    }
    Filesystem::Filesystem(Filesystem &&other)
    {
        *this = std::move(other);
    }
    Filesystem &Filesystem::operator=(Filesystem &&other)
    {
        if (this != &other)
        {
            delete m_root;
            m_root = other.m_root;
            m_current = other.m_current;
            other.m_current = nullptr;
            other.m_root = nullptr;
        }
        return *this;
    }
    Filesystem &Filesystem::operator+=(Resource *other)
    {
        *m_current += other;
        return *this;
    }

    Directory *Filesystem::change_directory(const std::string &name)
    {

        if (name.empty())
        {
            m_current = m_root;
        }
        else
        {
            vector<sdds::OpFlags> oflags;
            oflags.push_back(sdds::OpFlags::RECURSIVE);
            Directory *getDir = dynamic_cast<Directory *>(m_current->find(name, oflags));
            if (getDir)
            {
                m_current = getDir;
                return m_current;
            }
            else
            {
                throw std::invalid_argument("Cannot change directory! " + name + " not found.");
            }
        }
        return m_current;
    }
    Directory *Filesystem::get_current_directory() const
    {
        return m_current;
    }
    Filesystem::~Filesystem()
    {
        delete m_root;
    }
}
