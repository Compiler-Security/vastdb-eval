# Database Schema Documentation (Framework)

This folder contains the framework for documenting database **node definitions** and **relationship (edge) definitions**.

## How to use
- Create one section per node type and per relationship type.
- Keep property names and types consistent with your actual database.
- Add constraints, indexes, and example queries at the end.

---

## 1. Overview
- Database: <name>
- Purpose: <what this graph models>
- Version: <schema version>
- Owner: <team / person>

## 2. Naming Conventions
- Node labels: <e.g., PascalCase>
- Relationship types: <e.g., UPPER_SNAKE>
- Properties: <e.g., lower_snake>
- IDs: <e.g., uuid / natural key>

## 3. Node Definitions

### Node: <Label>
- Description: <what this node represents>
- Primary key: <property name>

Properties:
- `property_name`: <type> — <description> (required/optional)
- `property_name`: <type> — <description> (required/optional)

Example:
```cypher
CREATE (n:<Label> { property_name: "value" })
```

---

## 4. Relationship Definitions

### Relationship: <TYPE>
- From: <SourceLabel>
- To: <TargetLabel>
- Description: <what this relationship represents>

Properties:
- `property_name`: <type> — <description> (required/optional)

Example:
```cypher
MATCH (a:<SourceLabel>), (b:<TargetLabel>)
CREATE (a)-[:<TYPE> { property_name: "value" }]->(b)
```

---

## 5. Constraints & Indexes

Constraints:
- <constraint name>: <definition>

Indexes:
- <index name>: <definition>

---

## 6. Example Queries

```cypher
// Example query here
MATCH (n:<Label>) RETURN n LIMIT 10
```
