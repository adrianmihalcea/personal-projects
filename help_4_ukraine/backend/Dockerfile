# BUILD
FROM maven:3.8.4-openjdk-17-slim AS builder
COPY pom.xml .
RUN mvn -e -B dependency:resolve
COPY src ./src
RUN mvn clean -e -B package -DskipTests
RUN ls /target/

# RUN
FROM openjdk:17
COPY --from=builder /target/backend-0.0.1-SNAPSHOT.jar .
ENTRYPOINT ["java", "-jar","backend-0.0.1-SNAPSHOT.jar"]
EXPOSE 8080

#

