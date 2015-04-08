# BMCProyecto1
Biologia Molecular Computacional Proyecto 1


{
	filename: carlos_gen,
    col: [ge1, ge2, ge3],
    val: [
    	{0, 33, 42.9, 1.1, 34.3},
        {... more data}
    ]
}

[
  {
  	"ge1": {
    	pos: 1,
        val: 0.13
     },
     "ge2": {
     	pos: 2,
        val: 0.27
     }
  },
  {
  	"ge3": {
    	pos: 1,
        val: 0.33
     },
     "ge4": {
     	pos: 2,
        val: 0.55
     },
     "ge1": {
     	pos: 3,
        val: 0.13
     },
  }
]

input >>> matriz_valores, order_gen_col
output >>> ge1,0.13|ge2,0.27

[ge1        |0.13| ge2       |0.27|    ge3    |0.33| ]
[ge3        |0.33| ge4       |0.55|    ge1    |0.13| ]

1. hacer una pantalla que guarde las frecuencias de recombinación por gen
	1.1 guardar en un json (Roy)
    1.2 optener de un json (Roy)
    
2. dibujar distintas secuencias de ADN con respecto a una cadena de string (Carlos)
	2.1 resultado de del proceso
    2.2 obtener de un json
    
3. Algoritmo para determinar la recombinación
	3.1 hacer algoritmo :(
        3.1.1 verificar consistencia
    	3.1.2 optner resultado
    3.2 guardar los resultados en un json
    3.3 enviar los resultado a ser dibujado

Optional

1. Hacer zoom al algoritmo
	1.1 nivel 1 de zoom
    1.2 nivel 2 de zoom
    
2. Diseño de la app
