#include "api.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

using namespace std;

template <class App>
class PageRankApp : public App
{
private:
	const typename App::value_t P = 0.85;
	const typename App::value_t DELTA = 1 - P;
	const typename App::value_t SVAL = 1;
	const typename App::value_t EPS = SVAL / 10000; // 0.0001; 

	typename App::value_t *delta;
public:
	PageRankApp(string dir, string value_dir): App(dir, string("pagerank_value.bin"), value_dir)
	{
		delta = (typename App::values_t)malloc(App::vtxs_size * sizeof(typename App::value_t));
	};

	void init(ID id, typename App::value_t &value)
	{
		value = 0;
		delta[id] = DELTA;
	}

	bool update(ID virtual_id, typename App::bor_t inbor, typename App::bor_t outbor, bor_cnt_t in_cnt, bor_cnt_t out_cnt,
		 const typename App::values_t rvalue, typename App::values_t wvalue)
	{
		if(fabs(delta[virtual_id]) < EPS)
			return true;

		typename App::value_t val = delta[virtual_id];

		if(out_cnt == 0)
			wvalue[virtual_id] +=  val;
		else
			wvalue[virtual_id] += (val /(typename App::value_t) out_cnt);
		
		val = val * P /(typename App::value_t)out_cnt;

		//cout << "calc: " << App::map_nto[virtual_id] << endl;
		/*
        	for (int i = 0; i < out_cnt; i++)
        	{
			delta[outbor[i]] += val;
		}
		//*/
		//*
		ID idval = 0;
		PG_Foreach(outbor, idval)
		{
			delta[idval] += val;
		}
		//*/
		delta[virtual_id] = 0;
        return false;
	}
};

int main(int argc, char *argv[])
{
        if (argc != 3 && argc != 4)
        {
                cout << "Usage ./pagerank <data dir> <generate value to dir> <print top(default 10)>" << endl;
                return 0;
        }

	int top_num = (argc == 4 ? top_num = atoi(argv[3]) : 10) ;

	string dir(argv[1]), to_dir(argv[2]);
/*
	cout << "normal para run" << endl; get_time(false);
	PageRankApp<UnSccDagApp<float>> upr(dir, to_dir);	
	upr.reset(false);
	upr.para_run();
	get_time(); cout << endl;
	upr.print_top(top_num);
//*/
//*
	cout << "para run" << endl; get_time(false);
	PageRankApp<SccDagApp<float>> pr(dir, to_dir);
	pr.reset(false);
	pr.para_run();
	get_time(); cout << endl;
	pr.print_top(top_num);
//*/
	return 0;
}
