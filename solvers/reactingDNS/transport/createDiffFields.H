volScalarField MW
(
    IOobject
    (
        "MW",
        runTime.constant(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("MW",dimensionSet (0,0,0,0,0,0,0),0.0)
);

volScalarField pUnit
(
    IOobject
    (
        "pUnit",
        runTime.constant(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("pUnit",dimensionSet (1,-1,-2,0,0,0,0),1.0)
);
volScalarField TUnit
(
    IOobject
    (
        "TUnit",
        runTime.constant(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("TUnit", dimTemperature, 1.0)
);
volScalarField DUnit
(
    IOobject
    (
        "DUnit",
        runTime.constant(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("DUnit",dimensionSet (0,2,-1,0,0,0,0),1.0)
);

volScalarField lambdaUnit
(
    IOobject
    (
        "lambdaUnit",
        runTime.constant(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("lambdaUnit",dimensionSet (1,1,-3,-1,0,0,0),1.0)
);

volScalarField MuUnit
(
    IOobject
    (
        "MuUnit",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("MuUnit",dimensionSet (1,-1,-1,0,0,0,0),1.0)
);

volScalarField diffsh
(
        IOobject
        (
        "diffsh",
		runTime.timeName(),
		mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
        //false
        ),
        mesh,
        dimensionedScalar("diffsh", dimEnergy/dimTime/dimVolume, 0.0)
);

//
volScalarField chemistryhsSource
(
        IOobject
        (
        "chemistryhsSource",
		runTime.timeName(),
		mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
        //false
        ),
        mesh,
        dimensionedScalar("chemistryhsSource", dimEnergy/dimTime/dimVolume, 0.0)
);




 
PtrList<gasHThermoPhysics> specieData(Y.size());
forAll(specieData, i)
{
	specieData.set
	(
		i,
		new gasHThermoPhysics
		(
			dynamic_cast<const reactingMixture<gasHThermoPhysics>&>
				(thermo).speciesData()[i]
		)
	);
}


//List<dimensionedScalar> Le(Y.size(),0.0);


PtrList<volScalarField> Diff(Y.size());
forAll(Y,i)
{
word name("Diff"+Y[i].name());
Diff.set
(
i,new volScalarField
    ( 
    IOobject
    (
        name,
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,  
    dimensionedScalar(name,dimensionSet (0,2,-1,0,0,0,0),0)
    )
);
}

PtrList<volScalarField> d(0.5*Y.size()*(Y.size()+1));
forAll(d,i)
{
word name("d"+i);
d.set
(
i,new volScalarField
    ( 
    IOobject
    (
        name,
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,  
    dimensionedScalar(name,dimensionSet (0,0,0,0,0,0,0),0)
    )
);
}

PtrList<volScalarField> X(Y.size());
forAll(X,i)
{
word name("X"+Y[i].name());
X.set
(
i,new volScalarField
    ( 
    IOobject
    (
        name,
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,  
    dimensionedScalar(name,dimensionSet (0,0,0,0,0,0,0),0)
    )
);
}

//**********************Binary Diffusion****************//
List<dimensionedScalar> Diff1(0.5*Y.size()*(Y.size()+1));
List<dimensionedScalar> Diff2(0.5*Y.size()*(Y.size()+1));
List<dimensionedScalar> Diff3(0.5*Y.size()*(Y.size()+1));
List<dimensionedScalar> Diff4(0.5*Y.size()*(Y.size()+1));
//******************************************************//

/*
volScalarField dk
(
           IOobject
           (
              "dk",
              runTime.timeName(),
              mesh,
              IOobject::NO_READ,
              IOobject::NO_WRITE
           ),
           mesh,
           dimensionedScalar("dk",dimensionSet (0,0,0,0,0,0,0),0.0)
);
*/
volScalarField DiNumerator
(
           IOobject
           (
              "DiNumerator",
              runTime.timeName(),
              mesh,
              IOobject::NO_READ,
              IOobject::NO_WRITE
           ),
           mesh,
           dimensionedScalar("DiNumerator",dimensionSet (0,0,0,0,0,0,0),0.0)
);

volScalarField DiDenominator
(
           IOobject
           (
              "DiDenominator",
              runTime.timeName(),
              mesh,
              IOobject::NO_READ,
              IOobject::NO_WRITE
           ),
           mesh,
           dimensionedScalar("DiDenominator",dimensionSet (0,0,0,0,0,0,0),0.0)
);

volScalarField Di
(
           IOobject
           (
              "Di",
              runTime.timeName(),
              mesh,
              IOobject::NO_READ,
              IOobject::NO_WRITE
           ),
           mesh,
           dimensionedScalar("Di",dimensionSet (0,2,-1,0,0,0,0),0.0)
);

volScalarField Hsi
(
           IOobject
           (
              "Hsi",
              runTime.timeName(),
              mesh,
              IOobject::NO_READ,
              IOobject::NO_WRITE
           ),
           mesh,
           dimensionedScalar("Hsi",dimensionSet (0,2,-2,0,0,0,0),0.0),
	       zeroGradientFvPatchScalarField::typeName
);

volScalarField fuelDi
(
           IOobject
           (
              "fuelDi",
              runTime.timeName(),
              mesh,
              IOobject::NO_READ,
              IOobject::AUTO_WRITE
           ),
           mesh,
           dimensionedScalar("fuelDi",dimensionSet (0,2,-1,0,0,0,0),0.0)
);

volVectorField DiffError
(
           IOobject
           (
              "DiffError",
              runTime.timeName(),
              mesh,
              IOobject::NO_READ,
              IOobject::NO_WRITE
           ),
           mesh,
           dimensionedVector("DiffError",dimensionSet (1,-2,-1,0,0,0,0),vector::zero)
);

volVectorField vk
(
           IOobject
           (
              "vk",
              runTime.timeName(),
              mesh,
              IOobject::NO_READ,
              IOobject::NO_WRITE
           ),
           mesh,
           dimensionedVector("vk",dimensionSet(1,0,-3,0,0,0,0),vector::zero)
);


