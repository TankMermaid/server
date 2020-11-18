--echo #
--echo # BASIC CASES
--echo #

EXPLAIN SELECT * FROM t1 WHERE t1.b <> 10;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1 WHERE t1.b > 10;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1 WHERE t1.b < 10;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1 WHERE t1.b = 10;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1 WHERE t1.b+2 = 10;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1 WHERE a IN (1,2,3);
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1 WHERE a BETWEEN 1 and 5;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

--echo # Equi-join condition
EXPLAIN SELECT * FROM t1,t2 WHERE t1.a=t2.b;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

--echo # CASE where selectivity estimates are not accurate

EXPLAIN SELECT * FROM t1 WHERE abs(t1.b) = 10;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

--echo #
--echo # The cardinality here would be not accurate because we don't have
--echo # selectivity for the predicate t1.b=10
--echo #

EXPLAIN SELECT * FROM t1 WHERE t1.c = 10;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

--echo #
--echo # BASIC CASES FOR AND CONJUNCT
--echo #

EXPLAIN SELECT * FROM t1 WHERE a >= 5 and b <= 6;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1 WHERE a BETWEEN 1 AND 5 AND b <= 6;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1,t2 WHERE t1.a > 5 AND t2.b < 10;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

--echo #
--echo # BASIC CASES FOR OR CONJUNCT
--echo #

EXPLAIN SELECT * FROM t1 WHERE a >= 5 OR a <= 2;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1 WHERE a BETWEEN 1 AND 5 OR a <= 0;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

--echo # The cardinality here would be not accurate because for the
--echo # OR conjunct all predicates can't be resolved to one column

EXPLAIN SELECT * FROM t1,t2 WHERE t1.a > 5 OR t2.b < 10;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1 WHERE a >= 5 OR b <= 6;
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

--echo #
--echo # COMPLEX CASES WITH AND-OR CONJUNCTS
--echo #

EXPLAIN SELECT * from t1,t2 WHERE t1.a=t2.b and (t1.a > 5 or t1.a < 10);
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * from t1,t2 WHERE t1.a=t2.b and (t1.a > 5 or t2.b < 10);
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * from t1,t2 WHERE t1.a=t2.b and (t1.a > 5 or t2.a < 10);
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * FROM t1
WHERE ((t1.a > 20  and t1.a < 30) or (t1.a > 40 and t1.a < 50)) AND
((t1.a > 25 and t1.a < 35) or (t1.a > 45 and t1.a < 55)) OR
((t1.a > 120 and t1.a < 130) or (t1.a > 140 and t1.a < 150)) AND
((t1.a > 125 and t1.a < 135) or (t1.a > 145 and t1.a < 155));
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN EXTENDED SELECT * from t1  WHERE ((t1.a IS NULL) or (t1.a = 5));
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

--echo #
--echo # Test with subquery
--echo #

EXPLAIN SELECT * from t1  WHERE t1.a = (select t2.a from t2 limit 1);
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;

EXPLAIN SELECT * from t1
WHERE t1.a = (select t2.a from t2 where t1.b=t2.b limit 1);
SELECT JSON_DETAILED(JSON_EXTRACT(trace, '$**.cardinality_accurate'))
FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;