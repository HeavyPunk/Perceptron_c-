#include <iostream>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdint.h>
using namespace std;


class NeuralNet{
	public:
	NeuralNet(uint8_t L, uint16_t n[]){
		CreateNeurons(L, n);
		CreateWeights(L, n);
		this->numLayers = L;
		this->neuronsInLayers.resize(L);
		for(uint8_t l=0; l<L; l++)this->neuronsInLayers[l] = n[l];

	}
	double debug_getNeurons(uint16_t neu, uint16_t state, uint16_t layer){
		return this->neurons[neu][state][layer];
	}

	double debug_getWeights(uint16_t n1, uint16_t n2, uint8_t L){
		return this->weights[n1][n2][L];
	}

	//Eeah, here is function for start the neural net
	void Do_it(uint16_t size, double data[]){
		for(int n=0; n<size; n++){
			neurons[n][0][0] = data[n];
			neurons[n][1][0] = Func(neurons[n][0][0]);
		}
		for(int L=1; L<numLayers; L++){
			for(int N=0; N<neuronsInLayers[L]; N++){
				double input = 0;
				for(int lastN=0; lastN<neuronsInLayers[L-1]; lastN++){
					input += neurons[lastN][1][L-1]*weights[lastN][N][L-1];
				}
				neurons[N][0][L] = input;
				neurons[N][1][L] = Func(input);

			}
		}
	}

	void getResult(uint16_t size, double *data){
		for(uint16_t r = 0; r<size; r++){
			data[r] = neurons[r][1][numLayers-1];
		}
	}
    void learnBackpropagation(double data, double ans){

    }
	private:
	//varibles:
	vector<vector<vector<double>>> neurons;
	vector<vector<vector<double>>> weights;
	uint8_t numLayers=0;
	vector<double> neuronsInLayers;

	double Func(double in){
		return 1/(1 + exp(-in));
	}
	double Func_p(double in){
		return exp(-in)/((1 + exp(-in))*(1+exp(-in)));
	}

	int MaxEl(uint16_t size, uint16_t arr[]){
		uint16_t m = arr[0];
		for(int i = 0; i<size;i++)if(arr[i] > m)m=arr[i];
		return m;
	}
	void CreateNeurons(uint8_t L, uint16_t n[]){
		neurons.resize(MaxEl(L, n));
		for(int i =0; i<MaxEl(L,n); i++){
			neurons[i].resize(3);
			for(int j=0; j<3; j++)neurons[i][j].resize(L);
		}
	}
	void CreateWeights(uint8_t L, uint16_t n[]){
		this->weights.resize(MaxEl(L, n));
		for(int i=0; i<MaxEl(L,n); i++){
			weights[i].resize(MaxEl(L,n));
			for(int j=0; j<MaxEl(L,n); j++){
				weights[i][j].resize(L-1);
			}
		}
	}
};

int main(int argc, char *argv[])
{
	uint16_t neurons[] = {3, 10, 3};
	double data[] = {10, 11, 3};
	NeuralNet net(3, neurons);
	double res[3] = {0};
	/*
	for(int n=0; n<d[0];n++){
		for(int L=0;L<3;L++){
			for(int st=0; st<3; st++)
				cout << net.debug_getNeurons(n,st,L);
				cout<<' ';
		}
		cout<<'\n';
	}
	*/
	net.Do_it(3, data);
	net.getResult(3, res);
	for(int i = 0; i<3; i++)cout <<res[i]<<endl;
}
