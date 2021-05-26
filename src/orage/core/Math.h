#ifndef Math_h
#define Math_h
#define M_PIl 3.141592653589793238462643383279502884L /* pi */

#include <iostream>

class Math{
	public :
		typedef std::pair<int, int> FRACTION;
		typedef std::vector<FRACTION> FRACTION_LIST;
		typedef std::vector<FRACTION_LIST> LISTOF_FRACTION_LIST;
		typedef std::map<float, FRACTION_LIST> I;
		static const LISTOF_FRACTION_LIST getFractions(const int n){
			I f;
			LISTOF_FRACTION_LIST output;
			for (int i = 1; i <= n; i++) {
				if (n % i == 0){
					for (int j = 1; j <= i; j++) {
						float key = j/(float)i;
						if(f.count(key)>0){
							f[key].push_back(std::make_pair(j, i));
						}else{
							f[key]={std::make_pair(j, i)};
						}
					}
				}
			}
			for (const auto& [key, value] : f) {
				output.push_back(value);
			}
			return output;
	}
};

#endif /* Math_h */