CREATE DATABASE gestion_autos;

CREATE TABLE propietario(
    id_propietario SERIAL PRIMARY KEY,
    nombre VARCHAR(255) NOT NULL,
    ap_paterno VARCHAR(255) NOT NULL,
    ap_materno VARCHAR(255) NOT NULL,
    fecha_nacimiento DATE NOT NULL, -- validar la fecha 
    edad INTEGER CHECK (edad > 18) NOT NULL --no aceptar menores de edad
);

CREATE TABLE telefono(
    id_telefono SERIAL,
    id_propietario INTEGER NOT NULL,
    telefono VARCHAR(255) NOT NULL,
    PRIMARY KEY (id_telefono,id_propietario),
    FOREIGN KEY (id_propietario) REFERENCES propietario(id_propietario)
);


CREATE TABLE auto(
    id_auto SERIAL PRIMARY KEY,
    id_propietario INTEGER NOT NULL,
    marca VARCHAR(255) NOT NULL,
    modelo INTEGER CHECK (modelo >= 2011) NOT NULL,  -- Año del modelo del auto, debe ser 2011 o posterior
    tipo VARCHAR(255) NOT NULL,
    color VARCHAR(255) NOT NULL,
    placa VARCHAR(255) NOT NULL,
    FOREIGN KEY (id_propietario) REFERENCES propietario(id_propietario)
);


CREATE TABLE accidente(
    no_accidente SERIAL,
    id_auto INTEGER NOT NULL,
    fecha DATE CHECK (fecha <= CURRENT_DATE) NOT NULL, -- validar fecha,  La fecha debe ser hoy o una fecha pasada; no se permiten fechas futuras.
    hora TIME NOT NULL,
    lugar VARCHAR(255) DEFAULT 'Tapachula' NOT NULL, -- lugar predeterminado en este caso es tapachula 
    PRIMARY KEY (no_accidente, id_auto),
    FOREIGN KEY (id_auto) REFERENCES auto(id_auto)
);

--------------------------------------------------------------------
--FUNCION O PL 

CREATE OR REPLACE FUNCTION calcular_edad() 
RETURNS TRIGGER AS $$
BEGIN
    NEW.edad := DATE_PART('year', AGE(NEW.fecha_nacimiento));
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_calcular_edad
BEFORE INSERT OR UPDATE ON propietario
FOR EACH ROW
EXECUTE FUNCTION calcular_edad();
