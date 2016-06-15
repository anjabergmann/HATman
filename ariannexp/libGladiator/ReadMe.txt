========================================================================
    BIBLIOTECA DE V�NCULOS DIN�MICOS: Informaci�n general del proyecto libGladiator
========================================================================

El Asistente para aplicaciones ha creado este archivo DLL de libGladiator.  
Archivo que contiene un resumen del contenido de los archivos que
componen la aplicaci�n de libGladiator.


libGladiator.vcproj
    Archivo de proyecto principal para proyectos de VC++ generados con el Asistente para aplicaciones. 
    Contiene informaci�n acerca de la versi�n de Visual C++ que gener� el archivo y 
    de las plataformas, configuraciones y caracter�sticas del proyecto
    seleccionadas con el Asistente para aplicaciones.

libGladiator.cpp
    Archivo DLL de c�digo fuente principal

	Cuando este archivo DLL se crea, no exporta s�mbolos. Por lo tanto, 
	no crear� un archivo .lib cuando se genere. Si desea que este proyecto 
	constituya una dependencia de otro proyecto, es preciso 
	agregar c�digo para exportar algunos s�mbolos del archivo DLL para crear 
	una biblioteca de exportaci�n, o establecer la propiedad Omitir biblioteca de entrada en S�, 
	en la p�gina de propiedades General de la carpeta Vinculador del cuadro de di�logo 
	P�ginas de propiedades del proyecto.

/////////////////////////////////////////////////////////////////////////////
Otros archivos est�ndar:

StdAfx.h, StdAfx.cpp
    Archivos utilizados para generar un archivo de encabezado precompilado (PCH)
    llamado libGladiator.pch, as� como un archivo de tipos precompilado llamado StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Notas adicionales:

El asistente para aplicaciones utiliza los comentarios "TODO:" para indicar las partes del c�digo fuente que
se deben agregar o personalizar.

/////////////////////////////////////////////////////////////////////////////
