package bcc266TP1.toy;

import java.util.Random;

public class Programas {
	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		new Programas();
	}
	
	public Programas() {
		RAM ram;
		CPU cpu;
		ram = new RAM();		
		cpu = new CPU();
		
		//programaAleatorio(ram, cpu, 2000000);
		programaMult(ram, cpu, 15, 150);
		//programaDiv(ram, cpu, 175, 4);
		//programaFat(ram, cpu, 10);
		//programaSomaMatriz(ram, cpu, 3);//3x3
		
	}
	
	private void programaAleatorio(RAM ram, CPU cpu, int qdeIntrucoes) {
		Instrucao[] umPrograma = new Instrucao[qdeIntrucoes];
		Random r = new Random();
		int tamanhoRAM = 1000;
		ram.criarRAM_aleatoria(tamanhoRAM);
		for(int i=0; i<(qdeIntrucoes-1); i++) {
			Instrucao inst = new Instrucao();
			inst.opcode = r.nextInt(2);
			inst.add1 = r.nextInt(tamanhoRAM); // quer dizer que a RAM possui 1k posicoes
			inst.add2 = r.nextInt(tamanhoRAM);
			inst.add3 = r.nextInt(tamanhoRAM);
			umPrograma[i] = inst;
		}
		
		Instrucao ultima = new Instrucao();
		ultima.opcode = -1;
		umPrograma[qdeIntrucoes-1] = ultima;
		
		cpu.setPrograma(umPrograma);
		cpu.iniciar(ram);
	}
	
	private void programaSomaMatriz(RAM ram, CPU cpu, int cardinalidade) {
		//gerando as matrizes de forma aleatoria
		int[][] matriz1 = new int[cardinalidade][cardinalidade];
		int[][] matriz2 = new int[cardinalidade][cardinalidade];
		Random r = new Random();
		for(int i=0; i<cardinalidade; i++) {
			for(int j=0; j<cardinalidade; j++) {
				matriz1[i][j] = r.nextInt(100);
				matriz2[i][j] = r.nextInt(100);
			}
		}
		
		//levando tudo pra RAM que é um vetor....
		//9 posicoes para matriz1 + 9 posicoes para matriz2
		//e 9 posicoes para matriz resultante...
		//zerar a RAM
		ram.criarRAM_vazia(27);		
		int endRam=0;
		for (int i=0; i<cardinalidade; i++) {
			for(int j=0; j<cardinalidade; j++) {
				Instrucao[] trecho1 = new Instrucao[3];
				Instrucao inst0 = new Instrucao();
				inst0.opcode = 4;
				inst0.add1 = 1;//registrador1
				inst0.add2 = matriz1[i][j];
				trecho1[0] = inst0;
				
				Instrucao inst1 = new Instrucao();
				inst1.opcode = 2;
				inst1.add1 = 1;//registrador1
				inst1.add2 = endRam;//ram[endRam]
				trecho1[1] = inst1;
				
				Instrucao inst3 = new Instrucao();
				inst3.opcode = -1;
				trecho1[2] = inst3;
				
				cpu.setPrograma(trecho1);
				cpu.iniciar(ram);
				
				endRam++;
			}			
		}
		
		for (int i=0; i<cardinalidade; i++) {
			for(int j=0; j<cardinalidade; j++) {
				Instrucao[] trecho2 = new Instrucao[3];
				Instrucao inst0 = new Instrucao();
				inst0.opcode = 4;
				inst0.add1 = 1;//registrador1
				inst0.add2 = matriz2[i][j];
				trecho2[0] = inst0;
				
				Instrucao inst1 = new Instrucao();
				inst1.opcode = 2;
				inst1.add1 = 1;//registrador1
				inst1.add2 = endRam;//ram[endRam]
				trecho2[1] = inst1;
				
				Instrucao inst3 = new Instrucao();
				inst3.opcode = -1;
				trecho2[2] = inst3;
				
				cpu.setPrograma(trecho2);
				cpu.iniciar(ram);
				
				endRam++;
			}			
		}
		endRam=0;
		int delta=9; // valor a ser respeitado entre matriz 1 e 2
		for (int i=0; i<cardinalidade; i++) {
			for(int j=0; j<cardinalidade; j++) {
				Instrucao[] trecho3 = new Instrucao[2];
				Instrucao inst1 = new Instrucao();
				inst1.opcode = 0;//sum
				inst1.add1 = endRam;//ram[endRam]
				inst1.add2 = endRam+delta;//ram[endRam+delta]
				inst1.add3 = endRam+(2*delta);//ram[endRam+(2*delta)]
				trecho3[0] = inst1;
										
				Instrucao inst2 = new Instrucao();
				inst2.opcode = -1;
				trecho3[1] = inst2;
				
				cpu.setPrograma(trecho3);
				cpu.iniciar(ram);
				
				endRam++;
			}			
		}
	}
	
	private void programaFat(RAM ram, CPU cpu, int fat) {
		
		//executar instrucao
		//-1 -> halt
		// 0 -> soma
		// 1 -> subtrai
		// 2 -> copia do registrador para RAM
		// 3 -> copia da RAM para o registrador
		// 4 -> salva conteudo externo no registrador
		// 5 -> obtem conteudo externo do registrador
					
		
		//10! = 10x9x8x7x6x5x4x3x2x1
		int j=1;
		
		for(int i=1; i<=fat;i++) {
			programaMult(ram, cpu,j,i);
						
			Instrucao[] trecho1 = new Instrucao[3];
			Instrucao inst0 = new Instrucao();
			inst0.opcode = 3;
			inst0.add1 = 1;//registrador1
			inst0.add2 = 0;// RAM[0]
			trecho1[0] = inst0;
			
			Instrucao inst2 = new Instrucao();
			inst2.opcode = 5;
			inst2.add1 = 1;//registrador1
			inst2.add2 = -1;
			inst2.add3 = -1;
			trecho1[1] = inst2;
			
			Instrucao inst3 = new Instrucao();
			inst3.opcode = -1;
			trecho1[2] = inst3;
			
			cpu.setPrograma(trecho1);
			cpu.iniciar(ram);
			
			j = inst2.add2;
		}
		
		Instrucao[] trecho2 = new Instrucao[3];
		Instrucao inst4 = new Instrucao();
		inst4.opcode = 3;
		inst4.add1 = 1;//registrador1
		inst4.add2 = 0;//ram[0]
		inst4.add3 = -1;
		trecho2[0] = inst4;
		
		Instrucao inst5 = new Instrucao();
		inst5.opcode = 5;
		inst5.add1 = 1;//registrador1
		inst5.add2 = -1;
		inst5.add3 = -1;
		trecho2[1] = inst5;
		
		Instrucao inst6 = new Instrucao();
		inst6.opcode = -1;
		trecho2[2] = inst6;
		
		cpu.setPrograma(trecho2);
		cpu.iniciar(ram);
		
		System.out.println("O resultado do fatorial eh: " + inst5.add2);
		
	}
	
	private void programaDiv(RAM ram, CPU cpu, int dividendo, int divisor) {
		//zerar a RAM
		ram.criarRAM_vazia(4);
		//Ex. dividir 14 / 3:
		//14-3=11 (1 sub)
		//11-3=8 (2 subs)
		//8-3=5 (3 subs)
		//5-3=2 (4 subs)
		//2-3 < 0 (halt)
		//resultado 4
		
		
		//executar instrucao
		//-1 -> halt
		// 0 -> soma
		// 1 -> subtrai
		// 2 -> copia do registrador para RAM
		// 3 -> copia da RAM para o registrador
		// 4 -> salva conteudo externo no registrador
		// 5 -> obtem conteudo externo do registrador
		
		Instrucao[] trecho1 = new Instrucao[5];
		Instrucao inst0 = new Instrucao();
		inst0.opcode = 4;
		inst0.add1 = 1;//registrador1
		inst0.add2 = dividendo;
		trecho1[0] = inst0;	
		
		Instrucao inst1 = new Instrucao();
		inst1.opcode = 2;
		inst1.add1 = 1;//registrador1
		inst1.add2 = 0;//ram[0]
		trecho1[1] = inst1;
		
		Instrucao inst2 = new Instrucao();
		inst2.opcode = 4;
		inst2.add1 = 2;//registrador2
		inst2.add2 = divisor;
		trecho1[2] = inst2;	
		
		Instrucao inst3 = new Instrucao();
		inst3.opcode = 2;
		inst3.add1 = 2;//registrador2
		inst3.add2 = 1;//ram[1]
		trecho1[3] = inst3;
		
		Instrucao inst4 = new Instrucao();
		inst4.opcode = -1;
		trecho1[4] = inst4;
		
		cpu.setPrograma(trecho1);
		cpu.iniciar(ram);
		
		Instrucao[] trecho2 = new Instrucao[3];
		Instrucao inst5 = new Instrucao();
		inst5.opcode = 4;
		inst5.add1 = 1;//registrador1
		inst5.add2 = 1;//num 1
		trecho2[0] = inst5;
		
		Instrucao inst6 = new Instrucao();
		inst6.opcode = 2;
		inst6.add1= 1;//registrador1
		inst6.add2 = 2;//ram[2]
		trecho2[1] = inst6;
		
		Instrucao inst7 = new Instrucao();
		inst7.opcode = -1;
		trecho2[2] = inst7;
		
		cpu.setPrograma(trecho2);
		cpu.iniciar(ram);
		
		while(dividendo > divisor) {
			Instrucao[] trecho3 = new Instrucao[5];
			Instrucao inst8 = new Instrucao();
			inst8.opcode = 1;//sub
			inst8.add1 = 0;//ram[0]
			inst8.add2 = 1;//ram[1]
			inst8.add3 = 0;//ram[0]
			trecho3[0] = inst8;
			
			Instrucao inst9 = new Instrucao();
			inst9.opcode = 0;//sum
			inst9.add1 = 2;//ram[2]
			inst9.add2 = 3;//ram[3]
			inst9.add3 = 3;//ram[3]
			trecho3[1] = inst9;
			
			Instrucao inst10 = new Instrucao();
			inst10.opcode = 3;
			inst10.add1 = 1;//registrador1
			inst10.add2 = 0;//ram[0]
			inst10.add3 = -1;//ram[1]
			trecho3[2] = inst10;
			
			Instrucao inst11 = new Instrucao();
			inst11.opcode = 5;
			inst11.add1 = 1;//registrador1
			inst11.add2 = -1;
			inst11.add3 = -1;
			trecho3[3] = inst11;
			
			Instrucao inst12 = new Instrucao();
			inst12.opcode = -1;
			trecho3[4] = inst12;
			
			cpu.setPrograma(trecho3);
			cpu.iniciar(ram);
			
			dividendo = inst11.add2;
		}
		
		Instrucao[] trecho4 = new Instrucao[3];
		Instrucao inst13 = new Instrucao();
		inst13.opcode = 3;
		inst13.add1 = 1;//registrador1
		inst13.add2 = 3;//ram[3]
		inst13.add3 = -1;
		trecho4[0] = inst13;
		
		Instrucao inst14 = new Instrucao();
		inst14.opcode = 5;
		inst14.add1 = 1;//registrador1
		inst14.add2 = -1;
		inst14.add3 = -1;
		trecho4[1] = inst14;
		
		Instrucao inst15 = new Instrucao();
		inst15.opcode = -1;
		trecho4[2] = inst15;
		
		cpu.setPrograma(trecho4);
		cpu.iniciar(ram);
		
		System.out.println("O resultado da divisao eh: " + inst14.add2);
		
	}
	
	
	private void programaMult(RAM ram, CPU cpu, int multiplicando, int multiplicador) {
		//executar instrucao
		//-1 -> halt
		// 0 -> soma
		// 1 -> subtrai
		// 2 -> copia do registrador para RAM
		// 3 -> copia da RAM para o registrador
		// 4 -> salva conteudo externo no registrador
		// 5 -> obtem conteudo externo do registrador
				
		//zerar a RAM
		ram.criarRAM_vazia(2);
		//Ex. multiplicar 15 x 10, ou seja, 15+15+15... (10vezes)
		Instrucao[] trecho1 = new Instrucao[3];
		Instrucao inst0 = new Instrucao();
		inst0.opcode = 4;
		inst0.add1 = 1;//registrador1
		inst0.add2 = multiplicando;
		trecho1[0] = inst0;		
				
		Instrucao inst1 = new Instrucao();
		inst1.opcode = 2;
		inst1.add1 = 1;//registrador1
		inst1.add2 = 1;//ram[1]
		trecho1[1] = inst1;
		
		Instrucao inst2 = new Instrucao();
		inst2.opcode = -1;
		trecho1[2] = inst2;
		
		cpu.setPrograma(trecho1);
		cpu.iniciar(ram);
		
		for(int i=0; i<multiplicador; i++) {
			Instrucao[] trecho2 = new Instrucao[2];
			Instrucao inst3 = new Instrucao();
			inst3.opcode = 0;
			inst3.add1 = 0;//ram[0]
			inst3.add2 = 1;//ram[1]
			inst3.add3 = 0;//ram[0]
			trecho2[0] = inst3;
			
			Instrucao inst4 = new Instrucao();
			inst4.opcode = -1;
			trecho2[1] = inst4;
			
			cpu.setPrograma(trecho2);
			cpu.iniciar(ram);
		}
		
		Instrucao[] trecho3 = new Instrucao[3];
		Instrucao inst5 = new Instrucao();
		inst5.opcode = 3;
		inst5.add1 = 1;//registrador1
		inst5.add2 = 0;//ram[0]
		inst5.add3 = -1;
		trecho3[0] = inst5;
		
		Instrucao inst6 = new Instrucao();
		inst6.opcode = 5;
		inst6.add1 = 1;//registrador1
		inst6.add2 = -1;
		inst6.add3 = -1;
		trecho3[1] = inst6;
		
		Instrucao inst7 = new Instrucao();
		inst7.opcode = -1;
		trecho3[2] = inst7;
		
		cpu.setPrograma(trecho3);
		cpu.iniciar(ram);
		
		System.out.println("O resultado da multiplicacao eh: " + inst6.add2);
		
	}

}
