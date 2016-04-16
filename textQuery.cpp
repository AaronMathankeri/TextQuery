#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <set>

using namespace std;

class QueryResult;
class TextQuery;

class TextQuery {

public:
      using line_num = std::vector<std::string>::size_type;
      TextQuery(string Filename); //constructor that takes file
      QueryResult query(const string&) const; //query takes a word
private:
      shared_ptr<vector<string> > file; //input
      map<string, shared_ptr<set<line_num> > > word_map; //hash
};



class QueryResult{
friend void print(const QueryResult&);
public:
      QueryResult(string s,
		  shared_ptr<set<TextQuery::line_num> > p,
		  shared_ptr<vector<string> > f):
	    sought(s), lines(p), file(f) { }

private:
      string sought; //word looking for
      shared_ptr<set <TextQuery::line_num> > lines; //line it's on
      shared_ptr<vector<string> > file; //input file
};

TextQuery::TextQuery(string Filename): file(new vector<string>)
{
      //string Filename = "randomText.txt";
      string text;
      ifstream myfile (Filename);
      if (myfile.is_open())
	    {
		  while ( getline(myfile, text) )
			{
			      file->push_back(text); //save line
			      int n = file->size() -1; //save line number
			      istringstream line(text); //separate into words
			      string word;

			      while ( line >> word) //for each word in that line
				    {
					  //if that word isn't already in the map
					  auto &lines = word_map[word];
					  if (!lines)
						lines.reset(new set<line_num>);
					  lines->insert(n);
			      
				    }
			}
	    }
}

QueryResult
TextQuery::query(const string &sought) const
{
       static shared_ptr<set<line_num>> nodata(new set<line_num>);

       auto loc = word_map.find(sought);
       if (loc == word_map.end())
	     return QueryResult( sought, nodata, file);
       else
	     return QueryResult(sought, loc->second, file);
	 
}

string make_plural(size_t ctr, const string &word, const string &ending)
{

      return (ctr > 1) ? word + ending : word;
}

void print(const QueryResult &qr)
{
      cout << qr.sought << " occurs " << qr.lines->size() << " "
	   << make_plural(qr.lines->size(), "time", "s") << endl;
      ///*
      for (auto num : *qr.lines)
	    cout << "\t (line " << num + 1 << ") "
	       << * (qr.file-> begin() + num) << endl;
      //*/
}

int main(int argc, char *argv[])
{
      cout << " This should be challenging and fun" << endl;
     
      string filename, searchword;
      //      filename = "randomText.txt";
      //      searchword = "experience";

      cout << "Filename? ";
      getline(cin, filename);
      
      cout << "What word do you want to query? ";
      getline(cin, searchword);

      TextQuery trialQuery(filename);
      print(trialQuery.query(searchword));
      return 0;
}


