#include "ExchangeRateGraph.h"


class BestRatePath
{
public:	
	BestRatePath(){}
	void FillRateArray(ExchangeRateGraph *graph)
	{
		vector<Vertex*> vecVrtx = graph->getVertices();
		
		for(int i = 0; i < vecVrtx.size();i++)
		{
			v.push_back(vecVrtx[i]->getName());
		}

		for(int i = 0; i < v.size(); i++)
		{
			Vertex * ver = graph->findVertex(v[i]);
			vector<double> flVec;
			vector<string> nextVerVec;
			for(int j = 0; j < v.size(); j++)
			{
				
				if(i == j)
				{
					flVec.push_back(0.0);
					nextVerVec.push_back("None");
				}
				else
				{
					Vertex *verEdge = new Vertex(v[j]);
					if(Edge *edge = ver->FindEdge(verEdge))
					{
						flVec.push_back(edge->getWeight());							
						nextVerVec.push_back(verEdge->getName());
					}
					else
					{						
						flVec.push_back(0.0);						
						nextVerVec.push_back("None");
					}
				}
			}
			rate.push_back(flVec);
			next.push_back(nextVerVec);
		}
	}

	void CalculateBestPath()
	{
		for(int k = 0; k < v.size(); k++)
		{			
			for(int i = 0; i < v.size(); i++)
			{				
				for(int j = 0; j < v.size(); j++)
				{
					if(j==i )
						continue;
					if(rate[i][j] < (rate[i][k] * rate[k][j]))
					{						
						rate[i][j] = rate[i][k] * rate[k][j];
						next[i][j] = next[i][k];
					}
						
					
				}
			}

		}
	}

	pair<double, string> GetBestPath(string src, string dest)
	{
		string path;
		double bestRate = 0.0;

		if(next.empty() || rate.empty())
			return make_pair(bestRate, string("NO PATH"));


		int srcIdx = -1, destIdx = -1;
		for(int i = 0; i < v.size(); i++)
		{
			if(v[i] == src)
				srcIdx = i;
			else if(v[i] == dest)
				destIdx = i;
		}

		if(srcIdx < 0 || destIdx < 0)
			return make_pair(bestRate, string("NO PATH"));


		if(next[srcIdx][destIdx] == "None")
		{
			return make_pair(bestRate, string("NO PATH"));
		}

		bestRate = rate[srcIdx][destIdx];
		path.append(src);
		while(src.compare(dest))
		{
			for(int i = 0; i < v.size(); i++)
			{
				if(v[i] == src)
					srcIdx = i;				
			}
			src = next[srcIdx][destIdx];
			path.append(";");
			path.append(src);
		}
		return make_pair(bestRate,path);
	}

	void DisplayBestRates(string srcExg, string srcCurr, string destExg, string destCurr, pair<double, string> bestRP)
	{
		string bestPath = bestRP.second;
		cout<<"BEST_RATES_BEGIN"<<" ";
		cout<<srcExg<<" "<<srcCurr<<" "<<destExg<<" "<<destCurr<<" "<<bestRP.first<<endl;
		for(int i=0; i < bestPath.size(); i++)
		{
			string exg, curr;
			while(bestPath[i]!=','&& i < bestPath.size())
			{
				exg.push_back(bestPath[i]);
				i++;
			}

			while(bestPath[i]!=';' && i < bestPath.size())
			{
				curr.push_back(bestPath[i]);
				i++;
			}

			cout<<exg<<curr<<endl;			
		}
		cout<<"BEST_RATES_END"<<endl;
	}
private:
	vector<vector<double>> rate;
	vector <vector<string>> next;
	vector<string> v;
};