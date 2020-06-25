/*------------------------------------------------------------------------
 * load_avg.c
 *              System load average information
 *
 * Copyright (c) 2020, EnterpriseDB Corporation. All Rights Reserved.
 *
 *------------------------------------------------------------------------
 */

#include <stdlib.h>

#include "postgres.h"
#include "system_stats.h"

void ReadLoadAvgInformations(Tuplestorestate *tupstore, TupleDesc tupdesc)
{
	Datum      values[Natts_load_avg_info];
	bool       nulls[Natts_load_avg_info];
	double	   load_avg[3];

	memset(nulls, 0, sizeof(nulls));

	/* Read the load average from kernel */
	if (getloadavg(&load_avg, lengthof(load_avg)) != 3)
	{
		ereport(DEBUG1, (errmsg("Error while getting loadavg information from kernel")));
		return;
	}

	values[Anum_load_avg_one_minute]   = Float4GetDatum((float4) load_avg[0]);
	values[Anum_load_avg_five_minutes] = Float4GetDatum((float4) load_avg[1]);
	values[Anum_load_avg_fifteen_minutes]  = Float4GetDatum((float4) load_avg[2]);
	nulls[Anum_load_avg_ten_minutes] = true;

	tuplestore_putvalues(tupstore, tupdesc, values, nulls);
}
