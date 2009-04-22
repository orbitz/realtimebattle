/************************************************************************
    $Id: configurationparser.cpp,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
    RTB - Team Framework: Framework for RealTime Battle robots to communicate efficiently in a team
    Copyright (C) 2004 The RTB- Team Framework Group: http://rtb-team.sourceforge.net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    $Log: configurationparser.cpp,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <ctype.h>
#include "configurationparser.h"

namespace Parser {

	using std::ifstream;
	using std::ostringstream;
	using std::streampos;
	using std::ios_base;
	using std::cerr;
	using std::endl;
	using std::cout;
	using std::getline;
	
	
	ConfigurationParser::ConfigurationParser() throw (bad_exception)
	{
	}
	
	enum parserstate {
		ISSECTION,	// A section name is being read
		ISKEY,		// A key is being read
		ISVALUE, 	// A value is being read
		BEFOREVALUE,	// The parser is waiting for a value enclosed in ""
		BEFORESECTION,	// The parser is waiting for a section (initial state)
		SORK,		// The parser is waiting for a section or a key
		BEFOREEOL,	// The parser is waiting for a comment or a newline
		COMMENT,	// A comment is being read
		QUOTE		// A quote (\) was read
	};
	
	bool ConfigurationParser::iscchar(int c) const throw ()
	{
		return isgraph(c) || c==' ' || c=='\t' || 
			c=='ß' || c=='ä' || c=='ö' || c=='ü' ||
			c=='Ä' || c=='Ö' || c=='Ü';
	}
	
	string ConfigurationParser::erase_wspace(const string& s) const throw (bad_exception)
	{
		string dst;
		int i;
		
		int start=0;
		int last=s.find_last_not_of(' ');

		dst.reserve(s.length());
		while(s[start]==' ') ++start;
		for(i=start;i<last+1;++i) {
			if(s[i]==' ') {
	
				// a sequence of spaces is treated as one
				while(i<last && s[i+1]==' ') ++i;
				
				dst.push_back(' ');
			} else {
				dst.push_back(s[i]);
			}
		}
		
		return dst;
	}
	
	void ConfigurationParser::addFile(const string& filename) throw (ParserException, bad_exception)
	{
		ostringstream section, key, value;
		string currentsection, currentkey, currentvalue;
		const string empty;
		parserstate state;
		char c;
		ifstream cfile;
		int line=1;
		
		cfile.exceptions(ios_base::badbit | ios_base::failbit);
		
		try {
			cfile.open(filename.c_str(), ios_base::in);
			state=BEFORESECTION;
			while(!cfile.eof()) {
				c=cfile.get();
				if(state==ISSECTION) {
					if(c==']') {
						currentsection=erase_wspace(section.str());
						// this line is necessary
						section.clear();
						section.str(empty);
						state=SORK;
					} else if(c=='\n') {
						// Syntax-Error
						throw ConfigurationParserException(filename, line, "Premature end of line (']' forgotten?)");
					} else if(iscchar(c)) {
						section << c;
					}
				} else if(state==ISKEY) {
					if(c=='=') {
						currentkey=erase_wspace(key.str());
						// this line is necessary
						key.clear();
						key.str(empty);
						state=BEFOREVALUE;
					} else if(c=='\n') {
						// Syntax-Error
						throw ConfigurationParserException(filename, line, "Premature end of line ('=\"value\"' is missing)");
					} else if(iscchar(c)) {
						key << c;
					}
				} else if(state==ISVALUE) {
					if(c=='"') {
						currentvalue=value.str();
						// this line is necessary
						value.clear();
						value.str(empty);
						_configurationData[currentsection][currentkey]=currentvalue;
						state=BEFOREEOL;
					} else if(c=='\n') {
						// Syntax-Error
						throw ConfigurationParserException(filename, line, "Premature end of line ('\"' forgotten?)");
					} else if(c=='\\') {
						state=QUOTE;
					} else if(iscchar(c)) {
						value << c;
					}
				} else if(state==QUOTE) {
					if(c=='\\') { value << '\\'; state=ISVALUE; }
					else if(c=='"') { value << '"'; state=ISVALUE; }
					else {
						throw ConfigurationParserException(filename, line, "Only '\\' or '\"' may follow '\\'");
					}
				} else if(state==BEFOREVALUE) {
					if(c=='"') {
						state=ISVALUE;
					} else if(!isspace(c)) {
						// Syntax-Error
						throw ConfigurationParserException(filename, line, "Premature end of line ('\"value\"' is missing)");
					}
				} else if(state==BEFOREEOL) {
					if(c=='\n') {
						state=SORK;
						++line;
					} else if(c=='#') {
						state=COMMENT;
					} else if(!isspace(c)) {
						// Syntax-Error
						throw ConfigurationParserException(filename, line, "Characters after a complete line ('#' forgotten?)");
					}
				} else if(state==COMMENT) {
					if(c=='\n') { state=SORK; ++line; }
				} else if(state==SORK) {
					if(c=='[') {
						state=ISSECTION;
					} else if(c=='#') {
						state=COMMENT;
					} else if(isgraph(c)) {
						state=ISKEY;
						key << c;
					} else if(c=='\n') {
						++line;
					}
				} else if(state==BEFORESECTION) {
					if(c=='[') {
						state=ISSECTION;
					} else if(c=='#') {
						state=COMMENT;
					} else if(isgraph(c)) {
						// Syntax Error 
						throw ConfigurationParserException(filename, line, "The configuration file has to begin with a section.");
					} else if(c=='\n') {
						++line;
					}
				}
			}
	
			if(state==ISSECTION) {
				// Syntax Error 
				throw ConfigurationParserException(filename, line, "Premature end of file: incomplete section name.");
			} else if(state==ISKEY) {
				// Syntax Error 
				throw ConfigurationParserException(filename, line, "Premature end of file: incomplete key name.");
			} else if(state==ISVALUE) {
				// Syntax Error 
				throw ConfigurationParserException(filename, line, "Premature end of file: incomplete value.");
			}
		} catch(ifstream::failure& e) {
			if(cfile.eof()) {
				// the exception was just the warning that we could get an eof.
				return;
			} else {
				throw ParserException("Error while reading/opening file '"+filename+"': "+e.what());
			}
		}
	}
	
	const map<string, map<string, string> >& ConfigurationParser::getConfigurationData() const throw ()
	{
		return _configurationData;
	}
	
}
