#include<iostream>
#include<vector>

int main() {
	std::vector<int> queue{1,2,3,4,5,6};
    // erase values
	for (auto it = queue.begin(); it != queue.end();) {
        std::cout<<"before erase, iter value" << *it << std::endl;
		if (*it > 2){
            it = queue.erase(it);
        } else { 
            it ++;
        }
        std::cout<<"after erase, iter value" << *it << std::endl;
	}
    // print erased queue
	for (auto it = queue.begin(); it != queue.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	return 0;
}
