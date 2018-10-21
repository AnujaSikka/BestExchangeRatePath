//#include "stdafx.h"
#include <iostream> 
#include <algorithm>
#include <set>
#include "BestRatePath.h"

using namespace std;

#define ExchangeRequest "exchange_rate_request"

void DisplayError()
{
	cout<<"Error in Input"<<endl;
}

						
int main()
{
	string inLine;
	vector<string> exchanges;
	vector<string> currencies;
	
	ExchangeRateGraph *ptrObjRateGraph = new ExchangeRateGraph();	

	while(getline(cin, inLine))	
	{
		if(inLine.empty())
		{
			DisplayError();
			continue;
		}
		if(!inLine.compare("clrscr"))
		{
			system("CLS");
			continue;
		}

		stringstream ss( inLine );
		string field;
		bool bExRateReq = false;
		if(getline(ss, field, ' '))
		{
			if (field.empty())
			{
				DisplayError();
				continue;
			}
			transform(field.begin(), field.end(), field.begin(), ::tolower);
			if(field.compare(ExchangeRequest))
			{
				EdgeTime stEdgeTime;
				string strDateTime =  field;
				if(!stEdgeTime.StrToTimeStamp(field))
				{
					DisplayError();
					continue;
				}

		
				string exchange, srcCurr, destCurr;
				double forFactor = 0.0, backFactor = 0.0;
				if(getline(ss, field, ' '))
				{
					if(field.empty())
					{
						DisplayError();
						continue;
					}
					transform(field.begin(), field.end(), field.begin(), ::tolower);
					exchange = field;
					if(find(exchanges.begin(), exchanges.end(), exchange)==exchanges.end())
						exchanges.push_back(exchange);					
				}
				if(getline(ss, field, ' '))
				{
					if(field.empty())
					{
						DisplayError();
						continue;
					}
					transform(field.begin(), field.end(), field.begin(), ::tolower);
					srcCurr = field;
					if(find(currencies.begin(), currencies.end(), srcCurr)==currencies.end())
						currencies.push_back(srcCurr);	
					
				}
				if(getline(ss, field, ' '))
				{
					if(field.empty())
					{
						DisplayError();
						continue;
					}
					transform(field.begin(), field.end(), field.begin(), ::tolower);
					destCurr = field;
					if(find(currencies.begin(), currencies.end(), destCurr)==currencies.end())
						currencies.push_back(destCurr);	
				}
				if(getline(ss, field, ' '))
				{
					if(field.empty())
					{
						DisplayError();
						continue;
					}
					forFactor = atof(field.c_str());
				}
				if(getline(ss, field, ' ') )
				{
					if(field.empty())
					{
						DisplayError();
						continue;
					}
					backFactor = atof(field.c_str());
				}

				if(backFactor*forFactor< 0 || backFactor*forFactor > 1)				
				{
					DisplayError();
					continue;
				}

				if(exchange.empty() || srcCurr.empty() || destCurr.empty())
				{
					DisplayError();
					continue;
				}
				Vertex *srcVertex = new Vertex(exchange+","+ srcCurr);;  				
				Vertex *destVertex = new Vertex(exchange+","+ destCurr);
				
				if(!srcVertex || !destVertex)
				{
					DisplayError();
					continue;
				}
				srcVertex->addEdge(destVertex, forFactor, stEdgeTime);
				destVertex->addEdge(srcVertex, backFactor, stEdgeTime);

				ptrObjRateGraph->insert(srcVertex);				
				ptrObjRateGraph->insert(destVertex);
			}
			else
			{
				BestRatePath *ptrObjBestRatePath = new BestRatePath();
				string srcExg, srcCurr, destExg, destCurr;
				
				if(getline(ss, field, ' '))
				{
					if(field.empty())
					{
						DisplayError();
						continue;
					}
					transform(field.begin(), field.end(), field.begin(), ::tolower);
					srcExg = field;
				}
				if(getline(ss, field, ' '))
				{
					if(field.empty())
					{
						DisplayError();
						continue;
					}
					transform(field.begin(), field.end(), field.begin(), ::tolower);
					srcCurr = field;
				}
				if(getline(ss, field, ' '))
				{
					if(field.empty())
					{
						DisplayError();
						continue;
					}
					transform(field.begin(), field.end(), field.begin(), ::tolower);
					destExg = field;
				}
				if(getline(ss, field, ' '))
				{
					if(field.empty())
					{
						DisplayError();
						continue;
					}
					transform(field.begin(), field.end(), field.begin(), ::tolower);
					destCurr = field;
				}

				ptrObjRateGraph->AddInterExgEdges(exchanges, currencies);
				ptrObjBestRatePath->FillRateArray(ptrObjRateGraph);
				ptrObjBestRatePath->CalculateBestPath();
				pair<double, string> bestRatePath = ptrObjBestRatePath->GetBestPath(srcExg+","+ srcCurr, destExg+","+ destCurr);

				ptrObjBestRatePath->DisplayBestRates(srcExg, srcCurr, destExg, destCurr, bestRatePath);

			}
		}
	}
	return 0;	
}
