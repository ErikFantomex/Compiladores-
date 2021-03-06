/* Nombre: AFD.h
   Autor: Luis Juventino Velasquez H.
   Fecha: 19/04/21
   Descripción: Archivo de cabecera de la clase AFD
*/

#include "Llave.h"

 /*****************************************************************************************
 **********************************   Constructores    ************************************
 *****************************************************************************************/
//*************************************************************************************
template <typename T_1,typename T_2>
Llave<T_1,T_2>::Llave()
{

}


//*************************************************************************************
template <typename T_1,typename T_2>
Llave<T_1,T_2>::Llave(const Llave<T_1,T_2> & ll2)
{
    (*this) = ll2;
}

 /*****************************************************************************************
 ************************************    Operadores    ************************************
 *****************************************************************************************/
//*************************************************************************************
template <typename T_1,typename T_2>
Llave<T_1,T_2> & Llave<T_1,T_2>::operator= (const Llave<T_1,T_2> & ll2)
{
    if(this == &ll2) return *this;

    this->llave = ll2.llave;
    this->valor = ll2.valor;

    return *this;
}


//*************************************************************************************
template <typename T_1,typename T_2>
bool Llave<T_1,T_2>::operator<(const Llave<T_1,T_2> & ll2) const
{
    return this->llave < ll2.llave;
}


//*************************************************************************************
template <typename T_1,typename T_2>
bool Llave<T_1,T_2>::operator>(const Llave<T_1,T_2> & ll2) const
{
    return this->llave > ll2.llave;
}


//*************************************************************************************
template <typename T_1,typename T_2>
bool Llave<T_1,T_2>::operator==(const Llave<T_1,T_2> & ll2) const
{
    return this->llave == ll2.llave;
}


//*************************************************************************************
template <typename T_1,typename T_2>
bool Llave<T_1,T_2>::operator>=(const Llave<T_1,T_2> & ll2) const
{
    return this->llave >= ll2.llave;
}


//*************************************************************************************
template <typename T_1,typename T_2>
bool Llave<T_1,T_2>::operator<=(const Llave<T_1,T_2> & ll2) const
{
    return this->llave <= ll2.llave;
}



 /*****************************************************************************************
 ********************************   Metodos y funciónes   *********************************
 *****************************************************************************************/
//*************************************************************************************
template <typename T_1,typename T_2>
void Llave<T_1,T_2>::Set(const T_1 & s)
{
    llave=s;
}

//*************************************************************************************
template <typename T_1,typename T_2>
void Llave<T_1,T_2>::Set(const T_1 & s, const T_2 &v)
{
    llave=s;
    valor=v;
}


//*************************************************************************************
template <typename T_1,typename T_2>
T_2 Llave<T_1,T_2>::GetValor() const
{
    return valor;
}


 /*****************************************************************************************
 *********************************   Metodos privados   ***********************************
 *****************************************************************************************/

