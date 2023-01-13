#include<stdio.h>
#include<stdlib.h>

struct fecha{
	short dia;
	short mes;
	short anio;
}nacimiento,ingreso;

struct empleados{
	int legajo;
	int borrado;
	int codigoEmpresa;
	char nomyape[30];
	char dni[10];
	short edad;
	char cargo[30];
	struct fecha nacimiento;
	struct fecha ingreso;
};

struct empresas{
		char nombre[30];
		int codigo;
		int borrado;
};

int codigoEnUso(int codigo){
	struct empresas empresa;
	FILE *fich;
	fich = fopen("empresas.bin","rb");
	if(fich==NULL){
		printf("Error de archivo\n");
		return -1;
	}
	fseek(fich, 0, SEEK_SET);
	while(fread(&empresa,sizeof(struct empresas),1,fich)){
		if(codigo==empresa.codigo && empresa.borrado!=0)
			return 1;
	}
	fclose(fich);
	return 0;
}
	
void alta(){
	struct empresas empresa;
	int fin=0;
	FILE *fich;
	fich = fopen("empresas.bin","ab");
	if(fich==NULL){
		printf("Error de archivo\n");
		return;
	}
	printf("\t---ALTA EMPRESA---\n\n");
	do{
		do{
			printf("Ingrese codigo empresa: ");
			fflush(stdin);
			scanf("%i",&empresa.codigo);
			if(codigoEnUso(empresa.codigo))
				printf("Codigo en uso, intente otro\n");
		}while(codigoEnUso(empresa.codigo));
		printf("Ingrese nombre empresa: ");
		fflush(stdin);
		gets(empresa.nombre);
		empresa.borrado=1;
		printf("nombre empresa: %s\n",empresa.nombre);
		printf("codigo empresa: %i\n",empresa.codigo);
		printf("Datos ingresados (para confirmar 1, para modificar 0)\n\n");
		scanf("%i",&fin);
	}while(fin!=1);

	fwrite(&empresa,sizeof(struct empresas),1,fich);
	fclose(fich);
	printf("Alta exitosa\n\n");
}

void baja(){
	struct empresas empresa;
	int codigo, fin=0,existe=0;
	FILE *fich;
	fich = fopen("empresas.bin","r+b");
	if(fich==NULL){
		printf("Error de archivo\n");
		return;
	}
	printf("Ingrese codigo empresa a dar baja: ");
	scanf("%i",&codigo);
	fread(&empresa,sizeof(empresa),1,fich);
	while(!feof(fich)){
		if(codigo == empresa.codigo && empresa.borrado!=0){
			printf("nombre empresa: %s\n",empresa.nombre);
			printf("codigo empresa: %i\n",empresa.codigo);
			printf("Datos a dar baja (para confirmar 1, para cancelar 0)\n\n");
			scanf("%i",&fin);
			if(fin==1){
				empresa.borrado=0;
				int pos = ftell(fich)-sizeof(empresa);
				fseek(fich,pos,0);
				fwrite(&empresa,sizeof(empresa),1,fich);
				printf("Baja exitosa\n\n");
				existe=1;
				break;
			}
		}
		fread(&empresa,sizeof(empresa),1,fich);
	}
	if(existe==0)
		printf("Codigo no encontrado\n\n");
	fclose(fich);
}

void modificar(){
	struct empresas empresa;
	int codigo, fin=0,existe=0;
	FILE *fich;
	fich = fopen("empresas.bin","r+b");
	if(fich==NULL){
		printf("Error de archivo\n");
		return;
	}
	printf("Ingrese codigo de empresa a modificar: ");
	scanf("%i",&codigo);
	fread(&empresa,sizeof(empresa),1,fich);
	while(!feof(fich)){
		if(codigo == empresa.codigo && empresa.borrado!=0){
			printf("nombre empresa: %s\n",empresa.nombre);
			printf("codigo empresa: %i\n",empresa.codigo);
			printf("Empresa a modificar (para confirmar 1, para cancelar 0)\n\n");
			scanf("%i",&fin);
			if(fin==1){	
				do{
					printf("Ingrese codigo empresa: ");
					fflush(stdin);
					scanf("%i",&empresa.codigo);
					if(codigoEnUso(empresa.codigo))
						printf("Codigo en uso, intente otro\n");
				}while(codigoEnUso(empresa.codigo));
				printf("Ingrese nombre empresa: ");
				fflush(stdin);
				gets(empresa.nombre);
				
				int pos = ftell(fich)-sizeof(empresa);
				fseek(fich,pos,0);
				fwrite(&empresa,sizeof(empresa),1,fich);
				printf("Modificacion exitosa\n\n");
				existe=1;
				break;
			}
		}
		fread(&empresa,sizeof(empresa),1,fich);
	}
	if(existe==0)
		printf("Codigo no encontrado\n\n");
	fclose(fich);
}

void ordenar(){
	printf("\t---ORDENAR LISTA EMPRESAS---\n\n");
	struct empresas empresa1,empresa2;
	FILE *fich;
	int i=0,j=0,band=1;
	long registros;
	fich = fopen("empresas.bin","r+b");
	if(fich==NULL){
		printf("Error, archivo no encontrado\n");
		return;
	}
	fseek(fich, 0, 2);
	registros=(ftell(fich)/sizeof(struct empresas))-1;
	fseek(fich,0,0);
	while(j<=registros-1 && band==1){
		band=0;
		i=0;
		while(i<=registros-1-j){
			fseek(fich,i*sizeof(empresa1),0);
			fread(&empresa1,sizeof(struct empresas),1,fich);
			fread(&empresa2,sizeof(struct empresas),1,fich);
			if(empresa1.codigo>empresa2.codigo){
				fseek(fich,i*sizeof(empresa1),0);
				fwrite(&empresa2,sizeof(struct empresas),1,fich);
				fwrite(&empresa1,sizeof(struct empresas),1,fich);
				band=1;
			}
			i++;
		}
	j++;
	}
	fclose(fich);
	printf("Lista de empresas ordenada correctamente\n\n");
}

void listado(){
	printf("\t---LISTA EMPRESAS---\n\n");
	struct empresas empresa;
	FILE *fich;
	fich = fopen("empresas.bin","rb");
	if(fich==NULL){
		printf("Error, archivo no encontrado\n");
		return;
	}
	fseek(fich, 0, 0);
	while(fread(&empresa,sizeof(struct empresas),1,fich)){
		if(empresa.borrado!=0){
			printf("nombre empresa: %s\n",empresa.nombre);
			printf("codigo empresa: %i\n",empresa.codigo);
			printf("\n");
		}
		
	}
	fclose(fich);
}

int legajoEnUso(int legajo){
	struct empleados empleado;
	FILE *fich;
	fich = fopen("empleados.bin","rb");
	if(fich==NULL){
		printf("Error de archivo\n");
		return -1;
	}
	fseek(fich, 0, SEEK_SET);
	while(fread(&empleado,sizeof(struct empleados),1,fich)){
		if(legajo==empleado.legajo && empleado.borrado!=0)
			return 1;
	}
	fclose(fich);
	return 0;
}

void altaEmpleado(){
	struct empleados empleado;
	struct empresas empresa;
	int fin=0,band=0;
	FILE *fich;
	fich = fopen("empleados.bin","ab");
	if(fich==NULL){
		printf("Error de archivo\n");
		return;
	}
	printf("\t---ALTA EMPLEADO---\n\n");
	do{
		do{
			printf("Ingrese legajo empleado: ");
			fflush(stdin);
			scanf("%i",&empleado.legajo);
			if(legajoEnUso(empleado.legajo))
				printf("Empleado ya existente\n");
		}while(legajoEnUso(empleado.legajo));
		printf("Ingrese nombre y apellido del empleado: ");
		fflush(stdin);
		gets(empleado.nomyape);
		empleado.borrado=1;
		
		FILE *fichEmpresas;
		fichEmpresas = fopen("empresas.bin","rb");
		if(fich==NULL){
			printf("Error, archivo empresas no encontrado\n");
			return;
		}
		do{
			printf("Seleccione la empresa: ");
			listado();
			printf("Seleccion: ");
			scanf("%i",&empleado.codigoEmpresa);
			fseek(fichEmpresas,0,0);
			while(fread(&empresa,sizeof(struct empresas),1,fichEmpresas) && band!=1){
				
				if(empleado.codigoEmpresa==empresa.codigo){
					band=1;
					
				}
					
			}
			if(band==0)
				printf("Por favor seleccione un codigo empresa de la lista\n");
		}while(band!=1);
		
		printf("Ingrese DNI: ");
		fflush(stdin);
		gets(empleado.dni);
		printf("Ingrese edad: ");
		fflush(stdin);
		scanf("%i",&empleado.edad);
		printf("Ingrese cargo: ");
		fflush(stdin);
		gets(empleado.cargo);
		printf("Ingreso fecha de nacimiento\n");
		fflush(stdin);
		printf("dia: ");
		fflush(stdin);
		scanf("%i",&empleado.nacimiento.dia);
		printf("mes: ");
		fflush(stdin);
		scanf("%i",&empleado.nacimiento.mes);
		printf("anio: ");
		fflush(stdin);
		scanf("%i",&empleado.nacimiento.anio);
		printf("Ingreso fecha de ingreso\n");
		printf("dia: ");
		fflush(stdin);
		scanf("%i",&empleado.ingreso.dia);
		printf("mes: ");
		fflush(stdin);
		scanf("%i",&empleado.ingreso.mes);
		printf("anio: ");
		fflush(stdin);
		scanf("%i",&empleado.ingreso.anio);
		printf("\n---Datos a ingresar---\n\n");
		printf("Nombre y Apellido: %s\n",empleado.nomyape);
		printf("Legajo Numero: %i\n",empleado.legajo);
		printf("DNI: %s\n",empleado.dni);
		printf("Cargo: %s\n",empleado.cargo);
		printf("Fecha de nacimiento: %i/%i/%i (%i anios)\n",empleado.nacimiento.dia,empleado.nacimiento.mes,empleado.nacimiento.anio,empleado.edad);
		printf("Fecha de ingreso: %i/%i/%i\n",empleado.ingreso.dia,empleado.ingreso.mes,empleado.ingreso.anio);	
		printf("\t\t(para confirmar 1, para modificar 0)\n\n");
		scanf("%i",&fin);
	}while(fin!=1);

	fwrite(&empleado,sizeof(struct empleados),1,fich);
	fclose(fich);
	printf("Alta exitosa\n\n");
}

void bajaEmpleado(){
	struct empleados empleado;
	int codigo, fin=0, existe=0;
	FILE *fich;
	fich = fopen("empleados.bin","r+b");
	if(fich==NULL){
		printf("Error de archivo\n");
		return;
	}
	printf("Ingrese legajo empleado a dar baja: ");
	scanf("%i",&codigo);
	fread(&empleado,sizeof(empleado),1,fich);
	while(!feof(fich)){
		if(codigo == empleado.legajo && empleado.borrado!=0){
			printf("Nombre empleado: %s\n",empleado.nomyape);
			printf("Legajo: %i\n",empleado.legajo);
			printf("Cargo: %s\n",empleado.cargo);
			printf("Datos a dar baja (para confirmar 1, para cancelar 0)\n\n");
			scanf("%i",&fin);
			if(fin==1){
				empleado.borrado=0;
				int pos = ftell(fich)-sizeof(empleado);
				fseek(fich,pos,0);
				fwrite(&empleado,sizeof(empleado),1,fich);
				printf("Baja exitosa\n\n");
				existe=1;
				break;
			}
		}
		fread(&empleado,sizeof(empleado),1,fich);
	}
	if(existe==0)
		printf("Codigo no encontrado\n\n");
	fclose(fich);
}

void modificarEmpleado(){
	struct empleados empleado;
	int codigo, fin=0,existe=0;
	FILE *fich;
	fich = fopen("empleados.bin","r+b");
	if(fich==NULL){
		printf("Error de archivo\n");
		return;
	}
	printf("Ingrese legajo de empleado a modificar: ");
	scanf("%i",&codigo);
	fread(&empleado,sizeof(empleado),1,fich);
	while(!feof(fich)){
		if(codigo == empleado.legajo && empleado.borrado!=0){
			printf("Nombre empleado: %s\n",empleado.nomyape);
			printf("Legajo empleado: %i\n",empleado.legajo);
			printf("Empleado a modificar (para confirmar 1, para cancelar 0)\n\n");
			scanf("%i",&fin);
			if(fin==1){	
				do{
					printf("Ingrese legajo de empleado: ");
					fflush(stdin);
					scanf("%i",&empleado.legajo);
					if(legajoEnUso(empleado.legajo))
						printf("Legajo en uso, intente otro\n");
				}while(legajoEnUso(empleado.legajo));
				printf("Ingrese nombre y apellido de empleado: ");
				fflush(stdin);
				gets(empleado.nomyape);
				
				int pos = ftell(fich)-sizeof(empleado);
				fseek(fich,pos,0);
				fwrite(&empleado,sizeof(empleado),1,fich);
				printf("Modificacion exitosa\n\n");
				existe=1;
				break;
			}
		}
		fread(&empleado,sizeof(empleado),1,fich);
	}
	if(existe==0)
		printf("Codigo no encontrado\n\n");
	fclose(fich);
}

void ordenarEmpleados(){
	struct empleados empleado1,empleado2;
	FILE *fich;
	int i=0,j=0,band=1;
	long registros;
	fich = fopen("empleados.bin","r+b");
	if(fich==NULL){
		printf("Error, archivo no encontrado\n");
		return;
	}
	fseek(fich, 0, 2);
	registros=(ftell(fich)/sizeof(struct empleados))-1;
	fseek(fich,0,0);
	while(j<=registros-1 && band==1){
		band=0;
		i=0;
		while(i<=registros-1-j){
			fseek(fich,i*sizeof(empleado1),0);
			fread(&empleado1,sizeof(struct empleados),1,fich);
			fread(&empleado2,sizeof(struct empleados),1,fich);
			if(empleado1.legajo>empleado2.legajo){
				fseek(fich,i*sizeof(empleado1),0);
				fwrite(&empleado2,sizeof(struct empleados),1,fich);
				fwrite(&empleado1,sizeof(struct empleados),1,fich);
				band=1;
			}
			i++;
		}
	j++;
	}
	fclose(fich);
}

void listadoEmpleado(){
	ordenarEmpleados();
	int codigo,band=0;
	printf("\t---LISTA EMPLEADOS---\n\n");
	struct empleados empleado;
	struct empresas empresa;
	FILE *fich;
	fich = fopen("empleados.bin","rb");
	if(fich==NULL){
		printf("Error, archivo no encontrado\n");
		return;
	}
	FILE *fichEmpresas;
		fichEmpresas = fopen("empresas.bin","rb");
		if(fich==NULL){
			printf("Error, archivo empresas no encontrado\n");
			return;
		}
		do{
			printf("Seleccione la empresa: ");
			listado();
			printf("Seleccion: ");
			scanf("%i",&codigo);
			fseek(fichEmpresas,0,0);
			while(fread(&empresa,sizeof(struct empresas),1,fichEmpresas) && band!=1){
				if(codigo==empresa.codigo)
					band=1;
			}
			if(band==0)
				printf("Por favor seleccione un codigo empresa de la lista\n");
		}while(band!=1);
	fseek(fich, 0, 0);
	while(fread(&empleado,sizeof(struct empleados),1,fich)){
		if(empleado.borrado!=0 && empleado.codigoEmpresa==codigo){
			printf("Legajo empleado: %i\n",empleado.legajo);
			printf("Nombre empleado: %s\n",empleado.nomyape);
			printf("DNI: %s\n",empleado.dni);
			printf("Edad: %i\n",empleado.edad);
			printf("Cargo: %s\n",empleado.cargo);
			printf("Antiguedad: %i anios\n",2022-empleado.ingreso.anio);
			printf("\n");
		}	
	}
	fclose(fich);
}

int main(){
	int menu;
	do{
		printf("\t---MENU---\n\n");
		printf("1)Alta empresa\n");
		printf("2)Baja empresa\n");
		printf("3)Modificar empresa\n");
		printf("4)Listado empresas\n");
		printf("5)Ordenar listado empresas\n\n");
		printf("6)Alta empleado\n");
		printf("7)Baja empleado\n");
		printf("8)Modificar empleado\n");
		printf("9)Listado empleado por empresa\n\n");
		printf("0)Salir\n\n");
		printf("Seleccion: ");
		scanf("%i",&menu);
		switch(menu){
			case 1:
				alta();
				break;
			case 2:
				baja();
				break;
			case 3:
				modificar();
				break;
			case 4:
				listado();
				break;
			case 5:
				ordenar();
				break;
			case 6:
				altaEmpleado();
				break;
			case 7:
				bajaEmpleado();
				break;
			case 8:
				modificarEmpleado();
				break;
			case 9:
				listadoEmpleado();
				break;
			case 0:
				return 0;
			default:
				printf("Seleccion fuera de rango\n");
				break;
		}
	}while(menu);
}
