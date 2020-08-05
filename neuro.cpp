#include "neuro.h"

NeuralNet::NeuralNet(uint8_t L, uint16_t *n) {
	CreateNeurons(L, n);
	CreateWeights(L, n);
	this->numLayers = L;
	this->neuronsInLayers.resize(L);
	for (uint8_t l = 0; l < L; l++)this->neuronsInLayers[l] = n[l];
}

double NeuralNet::debug_getNeurons(uint16_t neu, uint16_t state, uint16_t layer) {
	return this->neurons[neu][state][layer];
}

double NeuralNet::debug_getWeights(uint16_t n1, uint16_t n2, uint8_t L) {
	return this->weights[n1][n2][L];
}

void NeuralNet::Do_it(uint16_t size, double *data) {
	for (int n = 0; n < size; n++) {
		neurons[n][0][0] = data[n];
		neurons[n][1][0] = Func(neurons[n][0][0]);
	}
	for (int L = 1; L < numLayers; L++) {
		for (int N = 0; N < neuronsInLayers[L]; N++) {
			double input = 0;
			for (int lastN = 0; lastN < neuronsInLayers[L - 1]; lastN++) {
				input += neurons[lastN][1][L - 1] * weights[lastN][N][L - 1];
			}
			neurons[N][0][L] = input;
			neurons[N][1][L] = Func(input);
		}
	}
}

void NeuralNet::getResult(uint16_t size, double* data) {
	for (uint16_t r = 0; r < size; r++) {
		data[r] = neurons[r][1][numLayers - 1];
	}
}

void NeuralNet::learnBackpropagation(double* data, double* ans, double acs, double k) {  
	for (uint32_t e = 0; e < k; e++) {
		double* errors = new double[neuronsInLayers[numLayers - 1]];
		Do_it(neuronsInLayers[0], data);
		getResult(neuronsInLayers[numLayers - 1], errors);
		for (uint16_t n = 0; n < neuronsInLayers[numLayers - 1]; n++) {
			neurons[n][2][numLayers - 1] = (ans[n] - neurons[n][1][numLayers - 1]) * (neurons[n][1][numLayers - 1]) * (1 - neurons[n][1][numLayers - 1]);
		}
		for (uint8_t L = numLayers - 2; L > 0; L--) {
			for (uint16_t neu = 0; neu < neuronsInLayers[L]; neu++) {
				for (uint16_t lastN = 0; lastN < neuronsInLayers[L + 1]; lastN++) {
					neurons[neu][2][L] += neurons[lastN][2][L + 1] * weights[neu][lastN][L] * neurons[neu][1][L] * (1 - neurons[neu][1][L]);
					weights[neu][lastN][L] += neurons[neu][1][L] * neurons[lastN][2][L + 1] * acs;
				}
			}
		}
	}
}

double NeuralNet::Func(double in) {
	return 1 / (1 + exp(-in));
}

double NeuralNet::Func_p(double in) {
	return exp(-in) / ((1 + exp(-in)) * (1 + exp(-in)));
}

uint32_t NeuralNet::MaxEl(uint16_t size, uint16_t *arr) {
	uint16_t m = arr[0];
	for (int i = 0; i < size; i++)if (arr[i] > m)m = arr[i];
	return m;
}

void NeuralNet::CreateNeurons(uint8_t L, uint16_t *n) {
	neurons.resize(MaxEl(L, n));
	for (int i = 0; i < MaxEl(L, n); i++) {
		neurons[i].resize(3);
		for (int j = 0; j < 3; j++)neurons[i][j].resize(L);
	}
}

void NeuralNet::CreateWeights(uint8_t L, uint16_t *n) {
	this->weights.resize(MaxEl(L, n));
	for (int i = 0; i < MaxEl(L, n); i++) {
		weights[i].resize(MaxEl(L, n));
		for (int j = 0; j < MaxEl(L, n); j++) {
			weights[i][j].resize(L - 1);
		}
	}
}
