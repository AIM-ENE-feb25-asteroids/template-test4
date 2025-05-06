USE Autoverhuur
GO

--- KPI 1 test
SELECT 
    YEAR(HC.van_datum) AS Jaar,
    AT.omschrijving AS Autotype,
    COUNT(HC.contractnr) AS AantalHuurcontracten
FROM 
    Huurcontract HC
INNER JOIN 
    Auto A ON HC.krijgt_auto = A.autonr
INNER JOIN 
    Autotype AT ON A.autotype = AT.typecode
WHERE 
    YEAR(HC.van_datum) = 2024 
    AND AT.omschrijving = 'compacte gezinsauto'
GROUP BY 
    YEAR(HC.van_datum), 
    AT.omschrijving;

--- KPI 2 test
SELECT 
    YEAR(HC.van_datum) AS Jaar, 
    LOC.land AS Land, 
    AT.omschrijving AS Autotype, 
    SUM(DATEDIFF(DAY, HC.van_datum, HC.tot_datum) * AT.prijs_per_dag) AS TotaleOmzet,
    (
        SELECT 
            SUM(DATEDIFF(DAY, HC1.van_datum, HC1.tot_datum) * AT1.prijs_per_dag) * 1.05
        FROM 
            Huurcontract HC1
        INNER JOIN 
            auto A1 ON HC1.krijgt_auto = A1.autonr
        INNER JOIN 
            autotype AT1 ON A1.autotype = AT1.typecode
        INNER JOIN 
            locatie LOC1 ON HC1.locatie_ophalen = LOC1.locatiecode
        WHERE 
            YEAR(HC1.van_datum) = YEAR(HC.van_datum) - 1
            AND LOC1.land = LOC.land
            AND AT1.omschrijving = AT.omschrijving
    ) AS VorigJaarOmzetPlus5Procent
FROM 
    Huurcontract HC
INNER JOIN 
    auto A ON HC.krijgt_auto = A.autonr
INNER JOIN 
    autotype AT ON A.autotype = AT.typecode
INNER JOIN 
    locatie LOC ON HC.locatie_ophalen = LOC.locatiecode
WHERE 
    YEAR(HC.van_datum) = 2024
    AND AT.omschrijving = 'compacte gezinsauto'
    AND LOC.land = 'Belgi�'
GROUP BY 
    YEAR(HC.van_datum), LOC.land, AT.omschrijving;


--- KPI 3 test
SELECT 
    AT.omschrijving AS Autotype,
    AC.accessoirenaam AS Accessoire,
    LOC.land AS Land,
    SUM(WA.aantal * AC.prijs_per_dag * DATEDIFF(DAY, HC.van_datum, HC.tot_datum)) AS TotaleOmzet
FROM 
    Huurcontract HC
JOIN 
    Wenst_accessoire WA ON HC.contractnr = WA.huurcontract
JOIN 
    Accessoire AC ON WA.accessoire = AC.accessoirenaam
JOIN 
    Auto AU ON HC.krijgt_auto = AU.autonr
JOIN 
    Autotype AT ON AU.autotype = AT.typecode
JOIN 
    Locatie LOC ON HC.locatie_ophalen = LOC.locatiecode
WHERE 
    AC.accessoirenaam = 'COOLR'
    AND LOC.land = 'Belgi�'
    AND AT.omschrijving = 'elektrische gezinsauto'
    AND HC.van_datum <= HC.tot_datum
GROUP BY 
    AT.omschrijving, AC.accessoirenaam, LOC.land;


--- KPI 4 test
SELECT 
    LOC.land AS Land,
    AT.omschrijving AS Autotype,
    SUM(DATEDIFF(DAY, HC.van_datum, HC.tot_datum)) AS TotaalAantalDagen,
    SUM(DATEDIFF(DAY, HC.van_datum, HC.tot_datum) * AT.prijs_per_dag) AS TotaleHuurInkomsten,
    AVG(AT.prijs_per_dag) AS GemiddeldePrijsPerDag
FROM 
    Huurcontract HC
JOIN 
    Auto AU ON HC.krijgt_auto = AU.autonr
JOIN 
    Autotype AT ON AU.autotype = AT.typecode
JOIN 
    Locatie LOC ON HC.locatie_ophalen = LOC.locatiecode
WHERE 
    LOC.land = 'Belgi�'
    AND AT.omschrijving = 'compacte gezinsauto'
    AND HC.van_datum IS NOT NULL
    AND HC.tot_datum IS NOT NULL
GROUP BY 
    LOC.land, AT.omschrijving;