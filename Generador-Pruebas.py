import random
import string
import math

def generar_casos(nombre_archivo_s1, nombre_archivo_s2, longitud_s1, longitud_s2):
    caracteres = string.ascii_lowercase  # Solo letras minúsculas
    tipo_caso = random.choice(["repetidos", "transposiciones", "vacias"])  # Selección del caso "vacias", "repetidos", "transposiciones"

    if tipo_caso == "vacias":
        cadena1 = ""
        cadena2 = ""
    elif tipo_caso == "repetidos":
        # Generar una secuencia de caracteres repetidos
        longitud_repetida = max(1, math.ceil(longitud_s1 / 2))  # Al menos 1 carácter
        secuencia_repetida = "".join(random.choice(caracteres) for _ in range(longitud_repetida))
        cadena1 = (secuencia_repetida * ((longitud_s1 // longitud_repetida) + 1))[:longitud_s1]
        cadena2 = (secuencia_repetida * ((longitud_s2 // longitud_repetida) + 1))[:longitud_s2]
    elif tipo_caso == "transposiciones":
        cadena1 = "".join(random.choice(caracteres) for _ in range(longitud_s1))
        cadena2 = list(cadena1)
        if len(cadena2) > 1:
            i, j = random.sample(range(len(cadena2)), 2)
            cadena2[i], cadena2[j] = cadena2[j], cadena2[i]
        cadena2 = "".join(cadena2)
    else:
        cadena1 = "".join(random.choice(caracteres) for _ in range(longitud_s1))
        cadena2 = "".join(random.choice(caracteres) for _ in range(longitud_s2))

    # Escribir las cadenas en archivos
    with open(nombre_archivo_s1, "w") as archivo1:
        archivo1.write(cadena1)
    with open(nombre_archivo_s2, "w") as archivo2:
        archivo2.write(cadena2)

    print(f"Casos de prueba generados ({tipo_caso}) en {nombre_archivo_s1} y {nombre_archivo_s2}.")

# Configuración de los casos de prueba, aqui podremos cambiar el tamaño de los s1 y s2 para la longitud
longitud_s1 = 12
longitud_s2 = 12

generar_casos("s1.txt", "s2.txt", longitud_s1, longitud_s2)
