terraform {
  backend "s3" {
    # La configuration sera fournie via -backend-config
  }
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
          "dynamodb:PutItem",
          "dynamodb:Scan"
        ]
        Resource = [
          aws_dynamodb_table.chouine_info.arn,
          aws_dynamodb_table.chouine_parties.arn
        ]
      }
    ]
  })
}

resource "aws_api_gateway_rest_api" "chouine_api" {
  name        = "ChouineAPI"
  description = "API REST pour le jeu de la Chouine"

  endpoint_configuration {
    types = ["REGIONAL"]
  }
}

resource "aws_api_gateway_resource" "api" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  parent_id   = aws_api_gateway_rest_api.chouine_api.root_resource_id
  path_part   = "api"
}

# Resource /info
resource "aws_api_gateway_resource" "info" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  parent_id   = aws_api_gateway_resource.api.id
  path_part   = "info"
}

# Method GET /info (retrieve all items)
resource "aws_api_gateway_method" "get_all_info" {
  rest_api_id   = aws_api_gateway_rest_api.chouine_api.id
  resource_id   = aws_api_gateway_resource.info.id
  http_method   = "GET"
  authorization = "NONE"
}

# Integration GET /info avec DynamoDB Scan
resource "aws_api_gateway_integration" "get_all_info_integration" {
  rest_api_id             = aws_api_gateway_rest_api.chouine_api.id
  resource_id             = aws_api_gateway_resource.info.id
  http_method             = aws_api_gateway_method.get_all_info.http_method
  type                    = "AWS"
  integration_http_method = "POST"
  uri                     = "arn:aws:apigateway:${data.aws_region.current.name}:dynamodb:action/Scan"
  credentials             = aws_iam_role.api_gateway_dynamodb_role.arn

  request_templates = {
    "application/json" = jsonencode({
      TableName = aws_dynamodb_table.chouine_info.name
    })
  }
}

# Method Response GET /info
resource "aws_api_gateway_method_response" "get_all_info_response_200" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  resource_id = aws_api_gateway_resource.info.id
  http_method = aws_api_gateway_method.get_all_info.http_method
  status_code = "200"

  response_parameters = {
    "method.response.header.Access-Control-Allow-Origin" = true
  }
}

# Integration Response GET /info
resource "aws_api_gateway_integration_response" "get_all_info_integration_response" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  resource_id = aws_api_gateway_resource.info.id
  http_method = aws_api_gateway_method.get_all_info.http_method
  status_code = aws_api_gateway_method_response.get_all_info_response_200.status_code

  response_parameters = {
    "method.response.header.Access-Control-Allow-Origin" = "'*'"
  }

  response_templates = {
    "application/json" = "#set($inputRoot = $input.path('$'))\n$input.json('$.Items')"
  }

  depends_on = [aws_api_gateway_integration.get_all_info_integration]
}

# Resource /info/{index}
resource "aws_api_gateway_resource" "info_index" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  parent_id   = aws_api_gateway_resource.info.id
  path_part   = "{index}"
}

# Method GET /info/{index}
resource "aws_api_gateway_method" "get_info" {
  rest_api_id   = aws_api_gateway_rest_api.chouine_api.id
  resource_id   = aws_api_gateway_resource.info_index.id
  http_method   = "GET"
  authorization = "NONE"

  request_parameters = {
    "method.request.path.index" = true
  }
}

# Integration GET /info/{index} avec DynamoDB
resource "aws_api_gateway_integration" "get_info_integration" {
  rest_api_id             = aws_api_gateway_rest_api.chouine_api.id
  resource_id             = aws_api_gateway_resource.info_index.id
  http_method             = aws_api_gateway_method.get_info.http_method
  type                    = "AWS"
  integration_http_method = "POST"
  uri                     = "arn:aws:apigateway:${data.aws_region.current.name}:dynamodb:action/GetItem"
  credentials             = aws_iam_role.api_gateway_dynamodb_role.arn

  request_parameters = {
    "integration.request.path.index" = "method.request.path.index"
  }

  request_templates = {
    "application/json" = jsonencode({
      TableName = aws_dynamodb_table.chouine_info.name
      Key = {
        Id = {
          N = "$input.params('index')"
        }
      }
    })
  }
}

# Method Response GET /info/{index}
resource "aws_api_gateway_method_response" "get_info_response_200" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  resource_id = aws_api_gateway_resource.info_index.id
  http_method = aws_api_gateway_method.get_info.http_method
  status_code = "200"

  response_parameters = {
    "method.response.header.Access-Control-Allow-Origin" = true
  }
}

# Integration Response GET /info/{index}
resource "aws_api_gateway_integration_response" "get_info_integration_response" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  resource_id = aws_api_gateway_resource.info_index.id
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

# Method POST /info/{index}
resource "aws_api_gateway_method" "post_info" {
  rest_api_id   = aws_api_gateway_rest_api.chouine_api.id
  resource_id   = aws_api_gateway_resource.info_index.id
  http_method   = "POST"
  authorization = "NONE"
  api_key_required = true

  request_parameters = {
    "method.request.path.index" = true
  }
}

# Integration POST /info/{index} avec DynamoDB
resource "aws_api_gateway_integration" "post_info_integration" {
  rest_api_id             = aws_api_gateway_rest_api.chouine_api.id
  resource_id             = aws_api_gateway_resource.info_index.id
  http_method             = aws_api_gateway_method.post_info.http_method
  type                    = "AWS"
  integration_http_method = "POST"
  uri                     = "arn:aws:apigateway:${data.aws_region.current.name}:dynamodb:action/PutItem"
  credentials             = aws_iam_role.api_gateway_dynamodb_role.arn

  request_parameters = {
    "integration.request.path.index" = "method.request.path.index"
  }

  request_templates = {
    "application/json" = <<EOF
{
  "TableName": "${aws_dynamodb_table.chouine_info.name}",
  "Item": {
    "Id": {
      "N": "$input.params('index')"
    },
    "Text": {
      "S": "$input.path('$.Text')"
    }
  }
}
EOF
  }
}

# Method Response POST /info/{index}
resource "aws_api_gateway_method_response" "post_info_response_200" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  resource_id = aws_api_gateway_resource.info_index.id
  http_method = aws_api_gateway_method.post_info.http_method
  status_code = "200"

  response_parameters = {
    "method.response.header.Access-Control-Allow-Origin" = true
  }
}

# Integration Response POST /info/{index}
resource "aws_api_gateway_integration_response" "post_info_integration_response" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  resource_id = aws_api_gateway_resource.info_index.id
  http_method = aws_api_gateway_method.post_info.http_method
  status_code = aws_api_gateway_method_response.post_info_response_200.status_code

  response_parameters = {
    "method.response.header.Access-Control-Allow-Origin" = "'*'"
  }

  response_templates = {
    "application/json" = "{\"message\": \"Info créée/mise à jour avec succès\"}"
  }

  depends_on = [aws_api_gateway_integration.post_info_integration]
}

# Resource /partie
resource "aws_api_gateway_resource" "partie" {
  rest_api_id = aws_api_gateway_rest_api.chouine_api.id
  parent_id   = aws_api_gateway_resource.api.id
  path_part   = "partie"
}

# Method POST /partie
resource "aws_api_gateway_method" "post_partie" {
  rest_api_id   = aws_api_gateway_rest_api.chouine_api.id
  resource_id   = aws_api_gateway_resource.partie.id
  http_method   = "POST"
  authorization = "NONE"
  api_key_required = true
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
  "TableName": "ChouineParties",
  "Item": {
    "UserId": {
      "S": "$input.path('$.UserId')"
    },
    "Timestamp": {
      "S": "$input.path('$.Timestamp')"
    },
    "Version": {
      "S": "$input.path('$.Version')"
    },
    "Pioche": {
      "BS": [
        #foreach($item in $input.path('$.Pioche'))
        "$item"#if($foreach.hasNext),#end
        #end
      ]
    },
    "Starter": {
      "N": "$input.path('$.Starter')"
    },
    "J0": {
      "BS": [
        #foreach($item in $input.path('$.J0'))
        "$item"#if($foreach.hasNext),#end
        #end
      ]
    },
    "J1": {
      "BS": [
        #foreach($item in $input.path('$.J1'))
        "$item"#if($foreach.hasNext),#end
        #end
      ]
    }#if($input.path('$.A_J1') != "" && $input.path('$.A_J1').size() > 0),
    "A_J1": {
      "BS": [
        #foreach($item in $input.path('$.A_J1'))
        "$item"#if($foreach.hasNext),#end
        #end
      ]
    }#end#if($input.path('$.A_J0') != "" && $input.path('$.A_J0').size() > 0),
    "A_J0": {
      "BS": [
        #foreach($item in $input.path('$.A_J0'))
        "$item"#if($foreach.hasNext),#end
        #end
      ]
    }#end#if($input.path('$.Errors') != "" && $input.path('$.Errors').size() > 0),
    "Errors": {
      "SS": [
        #foreach($item in $input.path('$.Errors'))
        "$item"#if($foreach.hasNext),#end
        #end
      ]
    }#end
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

  triggers = {
    redeployment = sha1(jsonencode([
      aws_api_gateway_resource.info.id,
      aws_api_gateway_method.get_all_info.id,
      aws_api_gateway_integration.get_all_info_integration.id,
      aws_api_gateway_resource.info_index.id,
      aws_api_gateway_method.get_info.id,
      aws_api_gateway_integration.get_info_integration.id,
      aws_api_gateway_method.post_info.id,
      aws_api_gateway_integration.post_info_integration.id,
      aws_api_gateway_resource.partie.id,
      aws_api_gateway_method.post_partie.id,
      aws_api_gateway_integration.post_partie_integration.id,
    ]))
  }

  depends_on = [
    aws_api_gateway_integration.get_all_info_integration,
    aws_api_gateway_integration.get_info_integration,
    aws_api_gateway_integration.post_info_integration,
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

# API Key to protect POST endpoints
resource "aws_api_gateway_api_key" "chouine_api_key" {
  name        = "ChouineApiKey"
  description = "API key for protected POST endpoints"
  enabled     = true
}

# Usage plan attaching the API stage
resource "aws_api_gateway_usage_plan" "chouine_usage_plan" {
  name        = "ChouineUsagePlan"
  description = "Usage plan for API key-protected methods"

  api_stages {
    api_id = aws_api_gateway_rest_api.chouine_api.id
    stage  = aws_api_gateway_stage.chouine_stage.stage_name
  }
}

# Bind the API key to the usage plan
resource "aws_api_gateway_usage_plan_key" "chouine_usage_plan_key" {
  key_id        = aws_api_gateway_api_key.chouine_api_key.id
  key_type      = "API_KEY"
  usage_plan_id = aws_api_gateway_usage_plan.chouine_usage_plan.id
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

output "get_all_info_url" {
  value       = "${aws_api_gateway_stage.chouine_stage.invoke_url}/info"
  description = "URL GET /info (retrieve all items)"
}

output "get_info_url" {
  value       = "${aws_api_gateway_stage.chouine_stage.invoke_url}/info/{index}"
  description = "URL GET /info/{index}"
}

output "post_info_url" {
  value       = "${aws_api_gateway_stage.chouine_stage.invoke_url}/info/{index}"
  description = "URL POST /info/{index}"
}

output "post_partie_url" {
  value       = "${aws_api_gateway_stage.chouine_stage.invoke_url}/partie"
  description = "URL POST /partie"
}

# Helpful output: API key ID (value is managed in AWS and may not be retrievable if autogenerated)
output "api_key_id" {
  value       = aws_api_gateway_api_key.chouine_api_key.id
  description = "API Gateway API key ID (use AWS Console to view value if autogenerated)"
}
