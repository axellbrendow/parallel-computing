#include <stdio.h>
#include <stdlib.h>

/*
TEMPO SEQUENCIAL:

real    0m0.414s
user    0m0.185s
sys     0m0.221s

TEMPO CUDA:

real    0m2.330s
user    0m1.143s
sys     0m1.099s

*/

__global__ void scan_cuda(double* a, double *s, int width) {
  int t = threadIdx.x;
  int b = blockIdx.x*blockDim.x;
  double x;

  // cria vetor na memória local
  __shared__ double p[1024];

  // carrega elementos do vetor da memória global para a local
  if(b+t < width)
    p[t] = a[b+t];

  // espera que todas as threads tenham carregado seus elementos
  __syncthreads();

  for (int i = 1; i < blockDim.x; i *= 2) { // realiza o scan em log n passos
    if(t >= i) // verifica se a thread ainda participa neste passo
      x = p[t] + p[t-i]; // atribui a soma para uma variável temporária

    __syncthreads(); // espera threads fazerem as somas

    if(t >= i)
      p[t] = x; // copia a soma em definitivo para o vetor local

    __syncthreads();
  }

  if(b + t < width) // copia da memória local para a global
    a[b+t] = p[t];

  if(t == blockDim.x-1) // se for a última thread do bloco
    s[blockIdx.x+1] = a[b+t]; // copia o seu valor para o vetor de saída
} 

__global__ void add_cuda(double *a, double *s, int width) {
  int t = threadIdx.x;
  int b = blockIdx.x*blockDim.x;
  
  // soma o somatório do último elemento do bloco anterior ao elemento atual
  if(b+t < width)
    a[b+t] += s[blockIdx.x];
}

int main()
{
  int width = 40000000;
  int size = width * sizeof(double);

  int block_size = 1024;
  int num_blocks = (width-1)/block_size+1;
  int s_size = (num_blocks * sizeof(double));  
 
  double *a = (double*) malloc (size);
  double *s = (double*) malloc (s_size);

  for(int i = 0; i < width; i++)
    a[i] = i;

  double *d_a, *d_s;

  // alocar vetores "a" e "s" no device
  cudaMalloc((void **) &d_a, size);
  cudaMalloc((void **) &d_s, s_size);

  // copiar vetor "a" para o device
  cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);

  // definição do número de blocos e threads (dimGrid e dimBlock)
  dim3 dimGrid(num_blocks,1,1);
  dim3 dimBlock(block_size,1,1);

  // chamada do kernel scan
  scan_cuda<<<dimGrid,dimBlock>>>(d_a, d_s, width);

  // copiar vetor "s" para o host
  cudaMemcpy(s, d_s, s_size, cudaMemcpyDeviceToHost);

  // scan no host (já implementado)
  s[0] = 0;
  for (int i = 1; i < num_blocks; i++)
    s[i] += s[i-1];
 
  // copiar vetor "s" para o device
  cudaMemcpy(d_s, s, s_size, cudaMemcpyHostToDevice);

  // chamada do kernel da soma
  add_cuda<<<dimGrid,dimBlock>>>(d_a, d_s, width);

  // copiar o vetor "a" para o host
  cudaMemcpy(a, d_a, size, cudaMemcpyDeviceToHost);

  printf("\na[%d] = %f\n",width-1,a[width-1]);
  
  cudaFree(d_a);
  cudaFree(d_s);
}
