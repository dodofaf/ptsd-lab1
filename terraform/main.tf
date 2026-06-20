terraform {
  required_providers {
    aws = {
      source  = "hashicorp/aws"
      version = "~> 5.0"
    }
  }
}

provider "aws" {
  region                      = "us-east-1"
  access_key                  = "test"
  secret_key                  = "test"
  skip_credentials_validation = true
  skip_metadata_api_check     = true
  skip_requesting_account_id  = true
  s3_use_path_style           = true

  endpoints {
    s3       = "http://s3.localhost.localstack.cloud:4566"
    lambda   = "http://localhost:4566"
    sqs      = "http://localhost:4566"
    iam      = "http://localhost:4566"
    sts      = "http://localhost:4566"
  }
}

resource "aws_s3_bucket" "start_bucket" {
  bucket = "s3-start"
}

resource "aws_s3_bucket" "finish_bucket" {
  bucket = "s3-finish"
}

resource "aws_s3_bucket_lifecycle_configuration" "finish_bucket_lifecycle" {
  bucket = aws_s3_bucket.finish_bucket.id

  rule {
    id     = "clean-up-old-files"
    status = "Enabled"

    expiration {
      days = 30
    }
  }
}

resource "aws_sqs_queue" "notification_queue" {
  name = "file-copy-notifications"
}

data "archive_file" "lambda_zip" {
  type        = "zip"
  source_file = "${path.module}/lambda/copy_file.py"
  output_path = "${path.module}/lambda/copy_file.zip"
}

resource "aws_iam_role" "lambda_exec_role" {
  name = "lambda_exec_role"
  assume_role_policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Action = "sts:AssumeRole"
      Effect = "Allow"
      Principal = { Service = "lambda.amazonaws.com" }
    }]
  })
}

resource "aws_lambda_function" "copy_lambda" {
  filename         = data.archive_file.lambda_zip.output_path
  function_name    = "CopyS3FileLambda"
  role             = aws_iam_role.lambda_exec_role.arn
  handler          = "copy_file.lambda_handler"
  source_code_hash = data.archive_file.lambda_zip.output_base64sha256
  runtime          = "python3.9"

  environment {
    variables = {
      FINISH_BUCKET = aws_s3_bucket.finish_bucket.id
      SQS_QUEUE_URL = aws_sqs_queue.notification_queue.id
    }
  }
}

resource "aws_lambda_permission" "allow_s3" {
  statement_id  = "AllowExecutionFromS3Bucket"
  action        = "lambda:InvokeFunction"
  function_name = aws_lambda_function.copy_lambda.function_name
  principal     = "s3.amazonaws.com"
  source_arn    = aws_s3_bucket.start_bucket.arn
}

resource "aws_s3_bucket_notification" "start_bucket_trigger" {
  bucket = aws_s3_bucket.start_bucket.id

  lambda_function {
    lambda_function_arn = aws_lambda_function.copy_lambda.arn
    events              = ["s3:ObjectCreated:*"]
  }

  depends_on = [aws_lambda_permission.allow_s3]
}
