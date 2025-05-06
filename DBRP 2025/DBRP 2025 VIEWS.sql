use Autoverhuur
go

---KPI 1
CREATE VIEW HuurcontractenPerJaarPerType AS
SELECT 
	YEAR(Hc.van_datum) AS Jaar, 
	AT.omschrijving AS Autotype, 
	COUNT(Hc.contractnr) AS AantalHuurcontracten
FROM 
	Huurcontract HC 
INNER JOIN 
	auto A ON HC.krijgt_auto = A.autonr 
INNER JOIN 
	autotype AT ON a.autotype = AT.typecode
GROUP BY 
	YEAR(HC.van_datum), AT.omschrijving;
go

--- KPI 2
CREATE VIEW JaarlijkseOmzetPerLandEnAutotype AS
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
GROUP BY 
    YEAR(HC.van_datum), LOC.land, AT.omschrijving;
GO

--- KPI 3
CREATE VIEW OmzetAccessoirePerAutotypeEnLand AS
SELECT
    at.omschrijving AS Autotype,
    ac.accessoirenaam AS Accessoire,
    loc.land AS Land,
    SUM(wa.aantal * ac.prijs_per_dag * DATEDIFF(DAY, hc.van_datum, hc.tot_datum)) AS TotaleOmzet
FROM
    Huurcontract hc
JOIN
    Wenst_accessoire wa ON hc.contractnr = wa.huurcontract
JOIN
    Accessoire ac ON wa.accessoire = ac.accessoirenaam
JOIN
    Auto au ON hc.krijgt_auto = au.autonr
JOIN
    Autotype at ON au.autotype = at.typecode
JOIN
    Locatie loc ON hc.locatie_ophalen = loc.locatiecode
WHERE
    hc.van_datum <= hc.tot_datum
GROUP BY
    at.omschrijving,
    ac.accessoirenaam,
    loc.land;
go


--- KPI 4
CREATE VIEW GemiddeldeHuurinkomstenPerAutotype AS
SELECT
    l.land AS Land,
    at.omschrijving AS Autotype,
    DATEDIFF(DAY, h.van_datum, h.tot_datum) * at.prijs_per_dag AS TotaleHuurInkomsten,
    DATEDIFF(DAY, h.van_datum, h.tot_datum) AS AantalDagen,
    at.prijs_per_dag AS PrijsPerDag
FROM
    Huurcontract h
JOIN
    Auto au ON h.krijgt_auto = au.autonr
JOIN
    Autotype at ON au.autotype = at.typecode
JOIN
    Locatie l ON h.locatie_ophalen = l.locatiecode
WHERE
    h.van_datum IS NOT NULL
    AND h.tot_datum IS NOT NULL;
