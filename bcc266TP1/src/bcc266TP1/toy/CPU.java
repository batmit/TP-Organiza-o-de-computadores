package bcc266TP1.toy;

public class CPU {
	//a cpu pode ter dezenas de registradores
	private int registrador1;
	private int registrador2;
		
	private int PC;
	private Instrucao[] programa;
	private int opcode;
	
		
	public void setPrograma(Instrucao[] programaAux) {
		programa = programaAux;
	}
	
	public void iniciar(RAM ram) {
		opcode=0;
		PC=0;
		//aqui inicia o pipeline
		while(opcode!=-1) {
			//carregar instrucao
			Instrucao inst = programa[PC];
			
			//decodificar instrucao
			opcode = inst.opcode;
			
			//executar instrucao
			//-1 -> halt
			// 0 -> soma
			// 1 -> subtrai
			// 2 -> copia do registrador para RAM
			// 3 -> copia da RAM para o registrador
			// 4 -> salva conteudo externo no registrador
			// 5 -> obtem conteudo externo do registrador
			switch (opcode){
				//halt
				case -1: {
					System.out.println("programa terminou!!");
					ram.imprimir();
					break;
				}
				//soma
				case 0: {
					registrador1 = ram.getDado(inst.add1);
					registrador2 = ram.getDado(inst.add2);
					registrador1 +=registrador2;
					//salvar resultado
					ram.setDado(inst.add3, registrador1);
					System.out.println("Inst sum -> RAM posicao " + inst.add3 + " com conteudo " + registrador1);
					break;
				}
				//subtrai
				case 1: {
					registrador1 = ram.getDado(inst.add1);
					registrador2 = ram.getDado(inst.add2);
					registrador1 -=registrador2;
					//salvar resultado
					ram.setDado(inst.add3, registrador1);
					System.out.println("Inst sub -> RAM posicao " + inst.add3 + " com conteudo " + registrador1);
					break;
				}
				//copia do registrador para RAM
				//formato da instrucao [opcode,qual_registrador,end_ram,-1]
				case 2: {
					if(inst.add1==1) {
						ram.setDado(inst.add2, registrador1);
						System.out.println("Inst copy_reg_ram -> RAM posicao " + inst.add2 + " com conteudo " + registrador1);
						
					}else if(inst.add1==2) {
						ram.setDado(inst.add2, registrador2);
						System.out.println("Inst copy_reg_ram -> RAM posicao " + inst.add2 + " com conteudo " + registrador2);
						
					}
					break;
				}				
				//copia da RAM para o registrador
				//formato da instrucao [opcode,qual_registrador,end_ram,-1]
				case 3: {
					if(inst.add1==1) {
						registrador1 = ram.getDado(inst.add2);
						System.out.println("Inst copy_ram_reg -> Registrador1 com conteudo " + registrador1);
						
					}else if(inst.add1==2) {
						registrador2 = ram.getDado(inst.add2);
						System.out.println("Inst copy_ram_reg -> Registrador2 com conteudo " + registrador2);
					}
					break;
				}
				//salva conteudo externo no registrador
				//formato da instrucao [opcode,qual_registrador,conteudo_externo,-1]
				case 4: {
					if(inst.add1==1) {
						registrador1 = inst.add2;
						System.out.println("Inst copy_ext_reg -> Registrador1 com conteudo " + registrador1);
						
					}else if(inst.add1==2) {
						registrador2 = inst.add2;
						System.out.println("Inst copy_ext_reg -> Registrador2 com conteudo " + registrador2);
					}
					break;
				}
				//obtem conteudo externo do registrador
				//formato da instrucao [opcode,qual_registrador,-1,-1]
				// note que o conteudo do registrador vai ficar na inst
				//na posicao do 2o endereco, ou seja, [opcode,qual_registrador,conteudo_reg,-1]
				case 5: {
					if(inst.add1==1) {
						inst.add2 = registrador1;
						System.out.println("Inst obtain_reg -> Registrador1 com conteudo " + registrador1);
						
					}else if(inst.add1==2) {
						inst.add2 = registrador2;
						System.out.println("Inst obtain_reg -> Registrador2 com conteudo " + registrador2);
					}
					break;
				}
			
			}			
			
			PC++;
		}
	}

}
