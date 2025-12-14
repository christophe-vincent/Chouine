terraform {
  required_providers {
    aws = {
      source  = "hashicorp/aws"
      version = "~> 5.0"
    }
  }
}

provider "aws" {
  region = "eu-west-1" # Région Paris proche de Rennes
}

# Table ChouineParties
resource "aws_dynamodb_table" "chouine_parties" {
  name           = "ChouineParties"
  billing_mode   = "PAY_PER_REQUEST"
  hash_key       = "UserId"
  range_key      = "Timestamp"

  attribute {
    name = "UserId"
    type = "S"
  }

  attribute {
    name = "Timestamp"
    type = "S"
  }

  tags = {
    Name        = "Chouine"
    Environment = "production"
  }
}

# Table ChouineInfo
resource "aws_dynamodb_table" "chouine_info" {
  name         = "ChouineInfo"
  billing_mode = "PAY_PER_REQUEST"
  hash_key     = "Id"

  attribute {
    name = "Id"
    type = "N"
  }

  tags = {
    Name        = "Chouine"
    Environment = "production"
  }
}

# IAM Role pour API Gateway
resource "aws_iam_role" "api_gateway_dynamodb_role" {
  name = "APIGatewayDynamoDBRole"

  assume_role_policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Action = "sts:AssumeRole"
      Effect = "Allow"
      Principal = {
        Service = "apigateway.amazonaws.com"
      }
    }]
  })

  tags = {
    Name        = "Chouine"
    Environment = "production"
  }
}

# Policy pour accéder à DynamoDB
resource "aws_iam_role_policy" "api_gateway_dynamodb_policy" {
  name = "APIGatewayDynamoDBPolicy"
  role = aws_iam_role.api_gateway_dynamodb_role.id

  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [
      {
        Effect = "Allow"
        Action = [
          "dynamodb:GetItem",
          "dynamodb:PutItem"
        ]
        Resource = [
          aws_dynamodb_table.chouine_info.arn,
          aws_dynamodb_table.chouine_parties.arn
        ]
      }
    ]
  })
}

# Resource /info
resource "aws_api_gateway_resource" "info" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  parent_id   = aws_api_gateway_rest_api.chouine_api.root_resource_id
  path_part   = "info"
}

# Method GET /info
resource "aws_api_gateway_method" "get_info" {
  rest_api_id   = aws_api_gateway_rest_api.chouine_api.id
  resource_id   = aws_api_gateway_resource.info.id
  http_method   = "GET"
  authorization = "NONE"
}

# Integration GET /info avec DynamoDB
resource "aws_api_gateway_integration" "get_info_integration" {
  rest_api_id             = aws_api_gateway_rest_api.chouine_api.id
  resource_id             = aws_api_gateway_resource.info.id
  http_method             = aws_api_gateway_method.get_info.http_method
  type                    = "AWS"
  integration_http_method = "POST"
  uri                     = "arn:aws:apigateway:${data.aws_region.current.name}:dynamodb:action/GetItem"
  credentials             = aws_iam_role.api_gateway_dynamodb_role.arn

  request_templates = {
    "application/json" = jsonencode({
      TableName = aws_dynamodb_table.chouine_info.name
      Key = {
        Id = {
          N = "0"
        }
      }
    })
  }
}

# Method Response GET /info
resource "aws_api_gateway_method_response" "get_info_response_200" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  resource_id = aws_api_gateway_resource.info.id
  http_method = aws_api_gateway_method.get_info.http_method
  status_code = "200"

  response_parameters = {
    "method.response.header.Access-Control-Allow-Origin" = true
  }
}

# Integration Response GET /info
resource "aws_api_gateway_integration_response" "get_info_integration_response" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  resource_id = aws_api_gateway_resource.info.id
  http_method = aws_api_gateway_method.get_info.http_method
  status_code = aws_api_gateway_method_response.get_info_response_200.status_code

  response_parameters = {
    "method.response.header.Access-Control-Allow-Origin" = "'*'"
  }

  response_templates = {
    "application/json" = "#set($inputRoot = $input.path('$'))\n$input.json('$.Item')"
  }

  depends_on = [aws_api_gateway_integration.get_info_integration]
}

# Resource /partie
resource "aws_api_gateway_resource" "partie" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  parent_id   = aws_api_gateway_rest_api.chouine_api.root_resource_id
  path_part   = "partie"
}

# Method POST /partie
resource "aws_api_gateway_method" "post_partie" {
  rest_api_id   = aws_api_gateway_rest_api.chouine_api.id
  resource_id   = aws_api_gateway_resource.partie.id
  http_method   = "POST"
  authorization = "NONE"
}

# Integration POST /partie avec DynamoDB
resource "aws_api_gateway_integration" "post_partie_integration" {
  rest_api_id             = aws_api_gateway_rest_api.chouine_api.id
  resource_id             = aws_api_gateway_resource.partie.id
  http_method             = aws_api_gateway_method.post_partie.http_method
  type                    = "AWS"
  integration_http_method = "POST"
  uri                     = "arn:aws:apigateway:${data.aws_region.current.name}:dynamodb:action/PutItem"
  credentials             = aws_iam_role.api_gateway_dynamodb_role.arn

  request_templates = {
    "application/json" = <<EOF
{
  "TableName": "${aws_dynamodb_table.chouine_parties.name}",
  "Item": {
    "UserId": {
      "S": "$input.path('$.UserId')"
    },
    "Timestamp": {
      "S": "$input.path('$.Timestamp')"
    }
  }
}
EOF
  }
}

# Method Response POST /partie
resource "aws_api_gateway_method_response" "post_partie_response_200" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  resource_id = aws_api_gateway_resource.partie.id
  http_method = aws_api_gateway_method.post_partie.http_method
  status_code = "200"

  response_parameters = {
    "method.response.header.Access-Control-Allow-Origin" = true
  }
}

# Integration Response POST /partie
resource "aws_api_gateway_integration_response" "post_partie_integration_response" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  resource_id = aws_api_gateway_resource.partie.id
  http_method = aws_api_gateway_method.post_partie.http_method
  status_code = aws_api_gateway_method_response.post_partie_response_200.status_code

  response_parameters = {
    "method.response.header.Access-Control-Allow-Origin" = "'*'"
  }

  response_templates = {
    "application/json" = "{\"message\": \"Partie créée avec succès\"}"
  }

  depends_on = [aws_api_gateway_integration.post_partie_integration]
}

# Deployment de l'API
resource "aws_api_gateway_deployment" "chouine_deployment" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id

  depends_on = [
    aws_api_gateway_integration.get_info_integration,
    aws_api_gateway_integration.post_partie_integration
  ]

  lifecycle {
    create_before_destroy = true
  }
}

# Stage de l'API
resource "aws_api_gateway_stage" "chouine_stage" {
  deployment_id = aws_api_gateway_deployment.chouine_deployment.id
  rest_api_id   = aws_api_gateway_rest_api.chouine_api.id
  stage_name    = "prod"
}

# Data source pour la région
data "aws_region" "current" {}

# Outputs
output "chouine_parties_table_name" {
  value       = aws_dynamodb_table.chouine_parties.name
  description = "Nom de la table ChouineParties"
}

output "chouine_parties_table_arn" {
  value       = aws_dynamodb_table.chouine_parties.arn
  description = "ARN de la table ChouineParties"
}

output "chouine_info_table_name" {
  value       = aws_dynamodb_table.chouine_info.name
  description = "Nom de la table ChouineInfo"
}

output "chouine_info_table_arn" {
  value       = aws_dynamodb_table.chouine_info.arn
  description = "ARN de la table ChouineInfo"
}

output "api_endpoint" {
  value       = "${aws_api_gateway_stage.chouine_stage.invoke_url}"
  description = "URL de l'API Gateway"
}

output "get_info_url" {
  value       = "${aws_api_gateway_stage.chouine_stage.invoke_url}/info"
  description = "URL GET /info"
}

output "post_partie_url" {
  value       = "${aws_api_gateway_stage.chouine_stage.invoke_url}/partie"
  description = "URL POST /partie"
}
