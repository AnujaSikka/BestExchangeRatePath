
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "windows.h"
#include <time.h>

using namespace std;

enum month{Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};

class EdgeTime
{
private:
	SYSTEMTIME m_tm;
	
public:
	EdgeTime()
	{
		m_tm.wYear = -1;
		m_tm.wMonth = -1;
		m_tm.wDay = -1;
		m_tm.wDayOfWeek = -1;
		m_tm.wHour = -1;
		m_tm.wMinute = -1;
		m_tm.wSecond = -1;
		m_tm.wMilliseconds = -1;
	}

	bool StrToTimeStamp(string tp)
	{
		
		stringstream ss(tp);
		string field;
		SYSTEMTIME sysm_tm;
		GetSystemTime(&sysm_tm);
		if(getline(ss, field, '-'))
		{
			if(field.empty())
			{				
				return false;
			}
			m_tm.wYear = atoi(field.c_str());			
		}
		if(m_tm.wYear < 0 || sysm_tm.wYear < m_tm.wYear)
		{			
			return false;
		}
		if(getline(ss, field, '-'))
		{
			if(field.empty())
			{
				return false;
			}
			m_tm.wMonth = atoi(field.c_str());			
		}
		if(m_tm.wMonth > 12 || m_tm.wMonth < 0)
		{			
			return false;
		}
		if(getline(ss, field, 't'))
		{
			if(field.empty())
			{
				return false;
			}
			m_tm.wDay = atoi(field.c_str());
			switch(m_tm.wMonth)
			{
			case Jan:				
			case Mar:				
			case May:				
			case Jul:
			case Aug:
			case Oct:
			case Dec:
				if(m_tm.wDay < 0 || m_tm.wDay > 31)
				{
					return false;
				}
				break;			
			case Feb:
				if(m_tm.wYear%4 == 0 || m_tm.wYear%400 == 0)
				{
					if(m_tm.wDay < 0 || m_tm.wDay > 29)
					{
						return false;
					}
				}
				else
				{
					if(m_tm.wDay < 0 || m_tm.wDay > 28)
					{
						return false;
					}
				}
				break;
			default:
				if(m_tm.wDay < 0 || m_tm.wDay > 30)
				{
					return false;
				}				
			}
		}

		if(m_tm.wDay < 0 || m_tm.wDay > 31)
		{
			return false;
		}	
		if(getline(ss, field, ':'))
		{
			if(field.empty())
			{
				return false;
			}
			m_tm.wHour = atoi(field.c_str());			
		}
		if(m_tm.wHour > 24)
		{
			return false;
		}

		if(getline(ss, field, ':'))
		{
			if(field.empty())
			{
				return false;
			}
			m_tm.wMinute = atoi(field.c_str());			
		}
		if(m_tm.wMinute > 60 || m_tm.wMinute < 0)
		{
			return false;
		}
		if(getline(ss, field, '+'))
		{
			if(field.empty())
			{
				return false;
			}
			m_tm.wSecond = atoi(field.c_str());			
		}
		if(m_tm.wSecond > 60 || m_tm.wSecond < 0)
		{
			return false;
		}
		if(getline(ss, field, ':'))
		{
			m_tm.wHour += atoi(field.c_str());
		}
		if(getline(ss, field) && !field.empty())
		{
			m_tm.wMinute += atoi(field.c_str());
		}

		while(m_tm.wMinute > 59)
		{
			m_tm.wMinute -= 60;
			m_tm.wHour += 1;
		}

		while(m_tm.wHour > 23)
		{
			m_tm.wHour -= 24;
			m_tm.wDay += 1;
		}

		switch(m_tm.wMonth)
		{
		case Jan:
		case Mar:
		case May:
		case Jul:
		case Aug:
		case Oct:
		case Dec:
			{
				if(m_tm.wDay > 31)
				{
					m_tm.wDay = 1;
					m_tm.wMonth += 1;
				}				
			}
			break;
		case Feb:
			{
				if(m_tm.wYear%4 == 0 || m_tm.wYear%400 == 0)
				{
					if(m_tm.wDay > 29)
					{
						m_tm.wDay = 1;
						m_tm.wMonth += 1;
					}
				}
				else
				{
					if(m_tm.wDay > 28)
					{
						m_tm.wDay = 1;
						m_tm.wMonth += 1;
					}
				}				
			}
			break;
		default:
			{
				if(m_tm.wDay > 30)
				{
					m_tm.wDay = 1;
					m_tm.wMonth += 1;
				}
			}
		}
		
		if(m_tm.wMonth > Dec)
		{
			m_tm.wMonth = 1;
			m_tm.wYear += 1;
		}

		return true;

	}
		
	bool operator >(const EdgeTime& rhs)
	{
		if(m_tm.wYear > rhs.m_tm.wYear)
			return true;
		if(m_tm.wYear == rhs.m_tm.wYear)
		{
			if(m_tm.wMonth > rhs.m_tm.wMonth)
				return true;
			if(m_tm.wMonth == rhs.m_tm.wMonth)
			{
				if(m_tm.wDay > rhs.m_tm.wDay)
					return true;
				if(m_tm.wDay == rhs.m_tm.wDay)
				{
					if(m_tm.wHour > rhs.m_tm.wHour)
						return true;
					if(m_tm.wHour == rhs.m_tm.wHour)
					{
						if(m_tm.wMinute > rhs.m_tm.wMinute)
							return true;
						if(m_tm.wMinute == rhs.m_tm.wMinute)
						{
							if(m_tm.wSecond > rhs.m_tm.wSecond)
								return true;
						}
					}
				}
			}
		}

		return false;
	}
};


class Vertex;

class Edge
{
public:
    Edge(Vertex *org, Vertex *dest, double wgt, EdgeTime m_tm)
    {
        m_origin = org;
        m_destination = dest;
        m_weight = wgt;
		m_timestamp = m_tm;
    }

    Vertex* getOrigin() {return m_origin;}
    Vertex* getDestination() {return m_destination;}
    double getWeight() {return m_weight;}
	EdgeTime getTimeStamp () {return m_timestamp;}

	void UpdateEdge(double wgt, EdgeTime m_tm)
	{
		m_weight = wgt;
		m_timestamp = m_tm;
	}
	bool operator ==(const Edge& rhs) 
	{
		if((this->getOrigin() == const_cast<Edge&>(rhs).getOrigin()) && 
			(this->getDestination() == const_cast<Edge&>(rhs).getDestination()))
			return true;

		return false;
	}
private:
    Vertex* m_origin;
    Vertex* m_destination;
    double m_weight;
	EdgeTime m_timestamp;
};



class Vertex
{
public:
	Vertex(){}
    Vertex(string id)
    {
        m_name = id;
    }
    void addEdge(Vertex *v, double wgt, EdgeTime m_tm)
    {
        Edge newEdge(this, v, wgt, m_tm);
        m_edges.push_back(newEdge);
    }
	void UpdateEdge(Edge *edge, double wgt, EdgeTime m_tm)
	{
		edge->UpdateEdge(wgt, m_tm);
	}
	Edge* FindEdge(Vertex *v)
	{
		for(int i = 0; i < m_edges.size(); i++)
		{
			if(m_edges[i].getDestination()->getName() == v->getName())
				return &m_edges[i];
		}
		return NULL;
	}
    string getName() {return m_name;} 
    vector<Edge> getEdges() {return m_edges;}

private:
    string m_name;
    vector<Edge> m_edges;
};


class ExchangeRateGraph
{

public:	
	void insert(Vertex *v)
    {
		if(Vertex *ver = findVertex(v->getName()))
		{
			Edge eg = v->getEdges()[0];
			Edge *edge = ver->FindEdge(eg.getDestination());
			if(edge)
			{
				if(IsLaterTimeStamp(*edge, eg))
					ver->UpdateEdge(edge, eg.getWeight(), eg.getTimeStamp());

				return;
					
			}
			else
			{
				ver->addEdge(eg.getDestination(), eg.getWeight(), eg.getTimeStamp());
				return;
			}
		}
		AddVertex(v);        
    }
	void AddInterExgEdges(vector<string> exgs, vector<string> currs)
	{
		for(int i = 0; i < currs.size(); i++)
		{
			for(int j = 0; j < exgs.size(); j++)
			{
				string verName = exgs[j]+","+currs[i];
				if(Vertex *srcVer = findVertex(verName))
				{
					for(int k = 0; k < exgs.size(); k++)
					{
						if(j == k)
							continue;
						Vertex *destVer = new Vertex(exgs[k]+","+currs[i]);
						Edge *edge = srcVer->FindEdge(destVer);
						if(!edge)
						{
							EdgeTime st;
							st.StrToTimeStamp("00-00-00T00:00:00+00:00");
							srcVer->addEdge(destVer, 1.0, st);
						}
					}
				}
			}
		}
	}

	void AddVertex(Vertex *ver)
	{
		if(!ver)
			return;
		m_vertices.push_back(ver);
	}
	Vertex* findVertex(string name)
	{
		for(int i = 0; i < m_vertices.size(); i++)
		{
			if(m_vertices[i]->getName() == name)
				return m_vertices[i];
			
		}
		return NULL;
	}	
	bool IsLaterTimeStamp(Edge oldEdge, Edge newEdge)
	{
		EdgeTime stNewTime, stOldTime;
		stNewTime = newEdge.getTimeStamp();	
		stOldTime = oldEdge.getTimeStamp();	

		if(stNewTime > stOldTime)
			return true;

		return false;
	}
	vector<Vertex*> getVertices(){return m_vertices;}
	
private:
    vector<Vertex*> m_vertices;
};

