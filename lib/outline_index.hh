// this is a mini lib for indexing and query stuff
#ifndef IndexClassify_H
#define IndexClassify_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>		// used for binary search
#include <sstream>		// used for file name string stream
#include <string>               // used for files names
#include <stdlib.h>             // used for c_str() function
#include <iterator>     	// std::distance
//#include <cmath>		// for stat
#include "hashtable.hh"
#define maxNameLength 80
#define maxSeqLength 200

namespace khmer
{

typedef long ReadID;				//we may need unsinged long long if we have many reads
typedef unsigned int khmer_id;			//each khmer has a rank
typedef std::set<HashIntoType> tagged_khmer_set; //if we decide to change taggedkhmer from vector to set data str
typedef std::set<ReadID> set_read_id;
//typedef std::map<khmerID, set_read_id*> tags_to_reads_map;
//typedef std::map<khmerID,SetReadId*>;  //assing for each khmerID a set of reads Iet

class readNode
{
protected:
    unsigned long long _pageNum;
    char _name[maxNameLength];
    char _seq[maxSeqLength];
    unsigned int _nameLength;
    unsigned int _seqLength;
    void init() {
        _pageNum=0;
        _nameLength=_seqLength=0;
        for (int i=0; i<maxNameLength; i++) {
            _name[i]='*';
        }
        for (int i=0; i<maxSeqLength; i++) {
            _seq[i]='*';
        }
    }
public:
    //constructor
    readNode()	{
        init();
    }
    //destructor
    ~readNode() {
        ;
    }
    //accessors to get values
    unsigned long long	getPageNum()	{
        return _pageNum;
    }
    unsigned int 	getNameLength()	{
        return _nameLength;
    }
    unsigned int 	getSeqLength()	{
        return _seqLength;
    }
    char *	getName()	{
        return _name;
    }
    char *	getSeq()	{
        return _seq;
    }

    //accessors to set values
    void setPageNum(unsigned long long newPageNum)	{
        _pageNum=newPageNum;
    }
    void setNameLength(unsigned int newNameLength)	{
        _nameLength=newNameLength;
    }
    void setSeqLength(unsigned int newSeqLength)	{
        _seqLength=newSeqLength;
    }
    void setName(unsigned int newNameLength,std::string  newName)	{
        for (int i=0; i<newNameLength; i++) {
            _name[i]=newName[i];
        }
        this->setNameLength(newNameLength);
    }
    void setSeq(unsigned int newSeqLength,std::string  newSeq) {
        for (int i=0; i<newSeqLength; i++) {
            _seq[i]=newSeq[i];
        }
        this->setSeqLength(newSeqLength);
    }
    void nullfy()	{
        this->init();
    }
    // read IO
    void printPageNum()	{
        std::cout<<_pageNum;
    }
    void printName()	{
        for (unsigned int i=0; i<_nameLength; i++)	{
            std::cout<<_name[i];
        }
    }
    void printSeq()		{
        for (unsigned int i=0; i<_seqLength ; i++)	{
            std::cout<<_seq[i];
        }
    }
    void printPageNum(std::fstream& outFile) {
        outFile<<_pageNum;
    }
    void printName(std::fstream& outFile)	{
        for (unsigned int i=0; i<_nameLength; i++)	{
            outFile<<_name[i];
        }
    }
    void printSeq(std::fstream& outFile)	{
        for (unsigned int i=0; i<_seqLength ; i++)	{
            outFile<<_seq[i];
        }
    }

};

#define blockSizeRead sizeof(readNode)

//----------------------------------------------
void convertFastaToBin(std::string readsFileName,std::string readBinFileName);
//------------ Reads IO operations ---------------
void WriteToDiskHeader(std::fstream& readBinFile,unsigned long long* numReads);
void ReadFromDiskHeader(std::fstream& readBinFile,unsigned long long* numReads);

void WriteToDiskRead(std::fstream& readBinFile,readNode* read,unsigned long long pageNum);
void ReadFromDiskRead(std::fstream& readBinFile,readNode* read,unsigned long long pageNum);
//----------- tagSet IO operation ------------
void load_tagset(std::string infilename,std::vector<khmer::HashIntoType>& mykhmervector, unsigned int& k,unsigned int& _tag_density );
void print_tagset(std::string infilename,std::vector<khmer::HashIntoType>& mykhmervector,unsigned int save_ksize);
//----------- seedSet ----------------------
std::set<khmer::HashIntoType> consume_fasta_and_tag(std::string readsFileName,unsigned int save_ksize,unsigned int tag_density);
std::set<khmer::HashIntoType> consume_sequence_and_tag(std::string seq , unsigned int save_ksize, unsigned int _tag_density);

void save_seedset(std::string infilename, std::set<khmer::HashIntoType>& all_seeds, unsigned int& k,unsigned int& _tag_density );
void load_seedset(std::string infilename, std::vector<khmer::HashIntoType>& mykhmervector, unsigned int& k,unsigned int& _tag_density );
void print_seedset(std::string infilename, std::vector<khmer::HashIntoType>& mykhmervector, unsigned int save_ksize);
//---------- index ----------
void build_index(std::string readsBinFileName, unsigned int density,std::vector<khmer::HashIntoType>& sortedKhmerVector,unsigned int  save_ksize );
void load_index_header(std::string infilename ,unsigned int& num_tagged_khmer,std::vector<khmer::HashIntoType>& sorted_khmer,std::vector<unsigned int>& accumulated_sizes);
//--------- exact query -----------
void extract_tags_from_seq(std::string seq,unsigned int save_ksize,std::string infilename,std::set<khmer::HashIntoType>& qeuery_tagged_khmer);
void retrieve_read_ids_by_tag(std::string infilename,std::set<khmer::HashIntoType>& qeuery_tagged_khmer,std::set<long>& reads_ids );
void retrieve_read_by_id(std::string infilename, std::set<long>& reads_ids, std::vector<std::string>& reads);
void retrieve_read_by_id(std::string readsBinFileName, std::set<long>& reads_ids, std::map<long,std::string>& mymap);
unsigned int sim_measure(std::string seq1, std::string seq2, unsigned int save_ksize);
//------ sampling procedure --------
void samplefromfile_reads();
void samplefromfile_kmers();
void samplefrombinary();
//------- stat ----------
void create_stat(std::string statfilename,unsigned int num_seeds,unsigned int density);

};

#endif //IndexClassify_H
