/* Nombre: .cpp
   Autor:
   Fecha:
   Descripción:
*/

#include "ASintactico.h"



 /*****************************************************************************************
 **********************************   Constructores    ************************************
 *****************************************************************************************/

//*************************************************************************************
ASintactico::ASintactico(const char *dir/*, const char *dir2*/)
{
    InicializarAFD(dir,aConstantes,&listTonkenConst);
}

//*************************************************************************************
ASintactico::ASintactico(const char *dir, const char *dir2)
{
    InicializarAFD(dir,aConstantes,&listTonkenConst);
    InicializarAFD(dir2,aVariables,&listTonkenVar);

}

//*************************************************************************************
ASintactico::~ASintactico()
{
    delete [] listTonkenConst;
}

//*************************************************************************************
ASintactico::ASintactico(const ASintactico & a2)
{
    (*this) = a2;
}

 /*****************************************************************************************
 ************************************    Operadores    ************************************
 *****************************************************************************************/
//*************************************************************************************
ASintactico & ASintactico::operator= (const ASintactico & a2)
{
    if(this == &a2) return *this;

    aConstantes = a2.aConstantes;
    aVariables = a2.aVariables;
    delete [] listTonkenConst;
    delete [] listTonkenVar;

    listTonkenVar = new std::string [aVariables.GetNEstados()];
    listTonkenConst = new std::string [aConstantes.GetNEstados()];

    for(int i=0,n=a2.aConstantes.GetNEstados();i<n;++i)
        listTonkenConst[i] = a2.listTonkenConst[i];

    for(int i=0,n=a2.aVariables.GetNEstados();i<n;++i)
        listTonkenVar[i] = a2.listTonkenVar[i];

    return *this;
}


 /*****************************************************************************************
 ********************************   Metodos y funciónes   *********************************
 *****************************************************************************************/
//*************************************************************************************
void ASintactico::Reset()
{
    aConstantes.Reset();
    aVariables.Reset();
}

//*************************************************************************************
void ASintactico::Avanzar(const char &c)
{
    aConstantes.Avanzar(((int) c) + SANGRIA);
}

//*************************************************************************************
double ASintactico::GetValorTValue(const std::string &t) const
{
    return tablaTValue.Buscar(t);
}


//*************************************************************************************
std::string ASintactico::LeerBuffer(Buffer &buff, bool fin)
{
    if(buff[0] == ' ')
    {
        buff.Sacar(1);
        return T_ESPACIO;
    }

    std::string tokenCons =NOACEPTACION;
    std::string tokenVar = NOACEPTACION;
    int maxConst=0,maxVar=0;

    tokenCons = EvaluarAutomata(buff,aConstantes,maxConst,listTonkenConst);
    tokenVar = EvaluarAutomata(buff,aVariables,maxVar,listTonkenVar);

    if(!fin && (tokenCons == NOACEPTACION || tokenVar == NOACEPTACION) )
        return NOACEPTACION;
    if(fin && tokenCons == NOACEPTACION)
    {
        ReconocerToken(tokenVar,buff,maxVar);
        buff.Sacar(maxVar);
        return tokenVar;
    }
    if(fin && tokenVar == NOACEPTACION)
    {
        ReconocerToken(tokenCons,buff,maxConst);
        buff.Sacar(maxConst);
        return tokenCons;
    }

    if(tokenCons == T_ERROR)
    {
        ReconocerToken(tokenVar,buff,maxVar);
        buff.Sacar(maxVar);
        return tokenVar;
    }

    if(tokenVar == T_ERROR)
    {
        ReconocerToken(tokenCons,buff,maxConst);
        buff.Sacar(maxConst);
        return tokenCons;
    }

    if(maxConst  == maxVar)
    {
        ReconocerToken(tokenCons,buff,maxConst);
        buff.Sacar(maxConst);
        return tokenCons;
    }
    else
    {
        if(maxConst > maxVar)
        {
            ReconocerToken(tokenCons,buff,maxConst);
            buff.Sacar(maxConst);
            return tokenCons;
        }
        else
        {
            ReconocerToken(tokenVar,buff,maxVar);
            buff.Sacar(maxVar);
            return tokenVar;
        }
    }

}

 /*****************************************************************************************
 ************************************   Excepciónes   *************************************
 *****************************************************************************************/
//*************************************************************************************
const char * ASintactico::DireccionNula::what() const throw()
{
    return "No se encontr\242 la el archivo...";
}

//*************************************************************************************
const char * ASintactico::Grosero::what() const throw()
{
    return "Hay una funcion para saber si el estado es de aceptacion y no la usaste... Grosero...";
}

//*************************************************************************************
const char * ASintactico::TablaIncorrecta::what() const throw()
{
    return "Hay un error en el automata del archivo...";
}

 /*****************************************************************************************
 *********************************   Metodos privados   ***********************************
 *****************************************************************************************/

void ASintactico::ReconocerToken(std::string &t,Buffer & buf, int tam)
{
    if(t == "TValue")
    {
        t+='*';
        double _val;
        char valor[50];
        for(int i=0;i<tam;++i)
        {
            t+=buf[i];
            valor[i]=buf[i];
        }
        valor[tam] = '\0';

        _val = atof(valor);
        std::string _= valor;

        tablaTValue.Agregar(t,_val);
    }

}

//*************************************************************************************
std::string ASintactico::EvaluarAutomata(Buffer &buff, AFD &automata,
                int &tamCadMasLarga, std::string *listaToken)
{
    std::string token=NOACEPTACION;
    std::string _;

    tamCadMasLarga = 0;
    automata.Reset();
    for(int i=0, f = buff.CantidadCaracteres(); i<f ; ++i)
    {
        automata.Avanzar(buff[i]+SANGRIA);

        if(automata.estadoAceptacion())
        {
            _ = token;
            token = listaToken[automata.GetEstadoAct() ];
            if(token == T_ERROR)
            {
                if(_ != NOACEPTACION)
                    token = _;

                return token;
            }
            tamCadMasLarga = i+1;
        }
    }
    return token;
}

//*************************************************************************************
void ASintactico::InicializarAFD(const char *dir, AFD & afd, std::string ** lTokens) const
{
    std::ifstream archivo(dir);
    HasImp< std::string,int > mapa;
    int caracDeCol[TAMALFABETO+2];
    if(!archivo)
    {
        archivo.close();
        throw DireccionNula();
    }


    //Leemos la primera linea para saber que caracter representa cada columna
    LigarColumnas(caracDeCol,archivo);

    //Le asignamos una fila a cada estado
    int e =NumerarEstados(mapa,archivo);

    //Dimencionar el automata y la lista de token
    DimencionarAFD(e,afd,lTokens);

    //llenamos la tabla, los estados de aceptacion y los T generados
    LlenarAFD(afd,lTokens,mapa,archivo,caracDeCol);

}

//*************************************************************************************
void ASintactico::LigarColumnas(int r[],std::ifstream &ent) const
{
    int i;
    char texto[2000];
    ent.getline(texto,2000);

    for( i=0; texto[i] != '\t' ; ++i);
    for(int col=0; texto[i] != '\0' ; ++i)
    {
        if(texto[i] == '\t') continue;
        //El blog de notas guarda los caracteres especiales de una manera 'especial'
            //¡Los guarda en tres partes¡ Hay que normalizarlos en uno
        r[col++]= ((int)NormalizarCaracter(texto,i))+SANGRIA;
    }
}

//*************************************************************************************
int ASintactico::NumerarEstados(HasImp<std::string,int> &mapa,std::ifstream &ent) const
{
    std::string sAux;
    char texto[2000];
    int nEstados;

    for(nEstados=0; !ent.eof() ; ++nEstados)
    {
        //Aqui no se debe normalizar el texto porque (A priori) los estados no tienen
            //carcteres especiales (Son de la forma q_0)
        ent.getline(texto,2000);

        sAux.clear();
        for(int j=0;texto[j]!='\t' && texto[j]!='*';++j)
            sAux+=texto[j];

        mapa.Agregar(sAux,nEstados);
    }
    return nEstados;
}

//*************************************************************************************
void ASintactico::DimencionarAFD(int &n,AFD &afd, std::string **lTokens) const
{
    //Nuestro alfabeto es de 68 elementos, pero para hacerlo más dinamico se declara de
        // 268 por el número de caracteres ascii
    afd.Dimencionar(n,TAMALFABETO+2,-1);
    (*lTokens) = new std::string[n];
}

//*************************************************************************************
void ASintactico::LlenarAFD(AFD & afd, std::string **lTokens, HasImp<std::string,int> & mapa,
        std::ifstream &ent, int relCol[]) const
{
    std::string tGen;
    bool esAcept;
    int indice;
    char texto[2000];
    ent.seekg(0, ent.beg);
    ent.getline(texto,2000);//Saltamos la primera linea(Los car. de las columnas)

    for(int fila=0; !ent.eof() ; ++fila)
    {
        indice=0;
        ent.getline(texto,2000);

        //Se lee el nombre del estado y se verifica si es de aceptacion o no
        esAcept = IdentificarEstado(texto,indice);

        if( esAcept )
        {
            tGen=GetTokenEstado(texto,indice);
            if(tGen=="ERROR") (*lTokens)[fila] = T_ERROR;
            else (*lTokens)[fila] = tGen;
        }
        else (*lTokens)[fila] = NOACEPTACION;
        afd.SetEstadosA(fila, esAcept);

        //Se llena la matriz de transicion
        LeerEstado(afd,fila,mapa,indice,relCol,texto);
    }
}


//*************************************************************************************
bool ASintactico::IdentificarEstado(const char ent[], int &i) const
{

    for(i=0;ent[i]!='\t' && ent[i] != '*' ;++i) ;
    return ent[i]=='*';
}

//*************************************************************************************
std::string ASintactico::GetTokenEstado(const char ent[], int &i) const
{
    std::string tGen="";
    if(ent[i] != '*') throw Grosero();

    for(++i;ent[i]!='\t';++i)
        tGen+=ent[i];

    return tGen;
}


//*************************************************************************************
void ASintactico::LeerEstado(AFD & afd, int &estado, HasImp<std::string,int>  &mapa, int &i,
        int relCol[],const char ent[] ) const
{
    std::string sAux;

    for(int colD=0,colR;ent[i]!='\0';)
    {
        if(ent[i] == '\t')
        {
            ++i;
            continue;
        }

        sAux.clear();
        while(ent[i]!='\t' && ent[i] != '\0')
            sAux+=ent[i++];

        colR=relCol[colD++];
        afd[estado][colR]= mapa.Buscar(sAux);
    }
}

 /*****************************************************************************************
 ***************************   Funciones externas y amigas   ******************************
 *****************************************************************************************/
