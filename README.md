# prog-paralela-trabalho
[![Actions Status](https://github.com/lffloyd/prog-paralela-trabalho/workflows/CMake/badge.svg)](https://github.com/lffloyd/prog-paralela-trabalho/actions)

Trabalho de implementação da disciplina de Laboratório de Programação Paralela do IC/UFF, período 2020.1.

## Ambiente
* **[cmake >= 3.18](https://cmake.org/install/)** - framework de build do projeto. Para usar a versão mais recente, [faça o build a partir do source](https://cmake.org/download/).

## Instalação
Execute ```./install.sh``` para obter as dependências do projeto (Google Test).

Para executar os scripts Python, será necessário executar ```./python_install.sh``` para obter as dependências. É necessário possuir Python 3 e Pip pré-instalados para executar tal ação.

## Build
Execute ```./build.sh``` para construir o código. Os executáveis serão gerados na pasta ```build``` na raiz do projeto.
Testes unitários serão executados automaticamente ao realizar o build.

# Resultados de execução
Para executar as implementações Prim aqui realizadas, calculando tempos de execução e salvando-os em tabelas para cada cenário, execute ```./build/src/ProgParalelaTrabalho_run <num threads> <lista de tamanhos de matriz quadrada>```, onde o primeiro argumento será o número de threads para executar a versão OpenMP e os argumentos restantes são inteiros representando os tamanhos n de matrizes a serem geradas para a coleta dos tempos. Todas as matrizes geradas nessa execução são quadradas.

O comando anterior irá gerar os resultados de tempos de execução na forma de tabelas CSV no diretório ```results``` na raiz deste projeto. Em seguida, para gerar gráficos a partir dos CSVs, execute:

```python3 src/graphs/graphs.py arquivo1.csv arquivo2.csv arquivo3.csv ...```, onde os argumentos da chamada são os nomes dos arquivos CSV gerados na etapa anterior. Os gráficos serão gerados no formato PNG no diretório ```results/graphs```.

### Links
* [Estruturando um projeto com CMake e testes unitários](https://raymii.org/s/tutorials/Cpp_project_setup_with_cmake_and_unit_tests.html)
* [Google Test Primer](https://github.com/google/googletest/blob/master/googletest/docs/primer.md)
